% Clear the console
clc;
% Clear all variables and functions memory
clear all;
% Close all open windows
close all;

% Select the image file
[ImgFilename ImgPathname] = uigetfile('*.bmp', 'Select the image on which data has to be hidden');
% Select the text file
[TxtFilename TxtPathname] = uigetfile('*.txt', 'Select the text file that contains the data to be hidden');

% Open and read the image
Img = imread(strcat(ImgPathname, ImgFilename));

% Open the text file
ftxt = fopen(strcat(TxtPathname, TxtFilename), 'rb');

% Find the text filesize
fseek(ftxt, 0, 'eof');
txtFilesize = ftell(ftxt);
frewind(ftxt);

% Read the text file
text = fread(ftxt, txtFilesize, 'uint8=>char');

% Find the width and height of the image
if ndims(Img) == 3
    % For color images
    [height width dim] = size(Img);
else
    % For Grayscale image
    [height width] = size(Img);
end

% Get the width and height
% width = input('Enter the width of the image : ');
% height = input('Enter the height of the image : ');

% Define a structure for holding the x and y coordinates
coordinates = repmat(struct('x', 0, 'y', 0), height, width);

% function CCDA_CENN = CCDA_Cen(width, height)
% width - Width of the image
% height - Height of the image

% Allocate memory for the array to check the coordinates is already scanned
isScanned = zeros(height, width);

% Allocate memory to store the coordinates one by one in CCDA_Cor order
coordinates(1 : width * height) = struct('x', 0, 'y', 0);

% Initial values
isFinishedScanning = 0;
iteration = 0;
count = 1;

while(isFinishedScanning ~= 1)
    iteration = iteration + 1;
    
    % Top left scanning
    startX = int32(width / 2);
    startY = int32(height / 2) - iteration + 1;
    
    while(startY ~= (height / 2) + 1)
        if ~((startX < 1) || (startX > width) || (startY < 1) || (startY > height))
            if isScanned(startY, startX) ~= 1
                isScanned(startY, startX) = 1;
                coordinates(count).x = startX;
                coordinates(count).y = startY;
                count = count + 1;
            end
        end
        startX = startX - 1;
        startY = startY + 1;
    end
    
    % Bottom left scanning
    startX = int32(width / 2) - iteration + 1;
    startY = int32(height / 2) + 1;
    
    while(startX ~= (int32(width / 2) + 1))
        if ~((startX < 1) || (startX > width) || (startY < 1) || (startY > height))
            if isScanned(startY, startX) ~= 1
                isScanned(startY, startX) = 1;
                coordinates(count).x = startX;
                coordinates(count).y = startY;
                count = count + 1;
            end
        end
        startX = startX + 1;
        startY = startY + 1;
    end
    
    % Bottom right scanning
    startX = int32(width / 2) + 1;
    startY = int32(height / 2) + iteration;
    
    while(startY ~= int32(height / 2))
        if ~((startX < 1) || (startX > width) || (startY < 1) || (startY > height))
            if isScanned(startY, startX) ~= 1
                isScanned(startY, startX) = 1;
                coordinates(count).x = startX;
                coordinates(count).y = startY;
                count = count + 1;
            end
        end
        startX = startX + 1;
        startY = startY - 1;
    end
    
    % Top right scanning
    startX = int32(width / 2) + iteration;
    startY = int32(height / 2);
    
    while(startX ~= int32(width / 2))
        if ~((startX < 1) || (startX > width) || (startY < 1) || (startY > height))
            if isScanned(startY, startX) ~= 1
                isScanned(startY, startX) = 1;
                coordinates(count).x = startX;
                coordinates(count).y = startY;
                count = count + 1;
            end
        end
        startX = startX - 1;
        startY = startY - 1;
    end
    
    if (count - 1) == (width * height)
        isFinishedScanning = 1;
    end
end

for row = 1 : width * height
    fprintf('Coordinates[%04d] = (%03d, %03d)\n', row, coordinates(row).x, coordinates(row).y);
end

fprintf('Length of data = %d\n', length(text));

input = text;

for data = 1 : length(text)
    pixel = double(Img(coordinates(data).x, coordinates(data).y));
    value = double(text(data));
    
    for bit = 7 : -1 : 0
        bitValue = bitshift(bitand(value, bitshift(1, bit)), -bit);
        if bitValue == 1
            pixel= bitor(pixel, 1);
        else
            if mod(pixel, 2) == 1
                pixel = pixel - 1;
            end
        end
    end
    Img(coordinates(data).x, coordinates(data).y) = uint8(pixel);
end
