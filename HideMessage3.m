% Clear the console
clc;
% Clear all variables and functions memory
clear all;
% Close all open windows
close all;

% Get the width and height
width = input('Enter the width of the image : ');
height = input('Enter the height of the image : ');

% Define a structure for holding the x and y coordinates
coordinates = repmat(struct('x', 0, 'y', 0), height, width);

% function CCDA_CORR = CCDA_Cor(width, height)
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
    startX = iteration;
    startY = 1;
    
    while(startX ~= 0)
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
    startX = 1;
    startY = height + 1 - iteration;
    
    while(startY ~= (height + 1))
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
    startX = width + 1 - iteration;
    startY = height;
    
    while(startX ~= (width + 1))
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
    startX = width;
    startY = iteration;
    
    while(startY ~= 0)
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
