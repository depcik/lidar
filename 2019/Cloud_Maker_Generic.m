%% LIDAR POINT CLOUD PROCESSING
% For files that give 
% Distance, horizontal motor position, and vertical motor position

% LIDAR Capstone Project
% Jaret Halberstadt, Theo Wiklund, Mark Heim, Michael Duncan

% Before you start change file name and other eg view, bounds, prompts...
clear
clc
close all

%% Input asks
prompt = 'What is the upward angle bound?\n';
up = input(prompt);
prompt = 'What is the downard angle bound?\n';
down = input(prompt);
prompt = 'What is the max sweep angle?\n';
side = input(prompt);
prompt = 'What is the distance cut off bound in m?\n If unknown guess big\n';
bounds = 1750; %this is an artifact of the original spahr auditorium point cloud
% we never got around to changing the code to not rely on it.

%% Data collection and analysis
STR = 'FSAE.txt'; %change tjos string to the name of the text file you want to read.
%type (STR); %Don't think this is needed
data = dlmread(STR);
% this reads data as: column 1 = raw distance, column 2 = horizontal motor
% angle, column 3 = vertical motor angle

%removing irrelevant raw distance data
high = data(:,1)  > 605;   % for rows where column 1 value is greater than
                           % any value you specify in centimeters
data(high,:) = [];   % remove those rows


low = data(:,1)  < 200;   % for rows where column 1 value less than any 
                          % value you specify in centimeters
data(low,:) = [];   % remove those rows; a distance of at least 2 would be 
                    % recommended in order to remove the frequent 1 values 
                    % of the rangefinder testing itself - refer to manual

% Radius checks for out of bounds and 1 outputs
radius = data(:,1);
in_or_out = radius < bounds & radius ~= 1;
radius = radius.*in_or_out; 

% Azimuthal rescales to sweep angle
Horizontal_motor = data(:,2);
azimuth = rescale(Horizontal_motor, -side,side);

% Elevation rescales to up and down angles
Vertical_motor = data(:,3);
elevation = rescale(Vertical_motor,down,up);

% Speherical to Cartesian, degrees to radians, cm to m
[x,z,y] = sph2cart(deg2rad(elevation),deg2rad(azimuth),radius/100);

%inter = Interpolation(x,y,z);
%x2 = inter(:,1);
%y2 = inter(:,2);
%z2 = inter(:,3);

%% 3d scatter plot and settings
figure (1)
% Create 3d plot with x,y,z points, dot size, color with respect to raw
% distance from rangefinder i.e. radius, filled circles
scatter3(x,-y,z,10,radius,'filled');

% Setup colormap and background color
colormap(jet); colorbar;
title(colorbar,'cm' ,'FontSize', 12);
title('M2SEC Formula Display Car Point Cloud','FontSize', 16);
set(gcf, 'Color', 'w');
% Label axes
xlabel('X Distance from rangefinder [m]', 'FontSize', 12);
ylabel('Y Distance from rangefinder [m]', 'FontSize', 12);
zlabel('Z Axis Elevation [m]', 'FontSize', 12);

%%adjust for outliers
       xlim([2.5 6]);
       ylim([-1 1.55]);
       zlim([-0.755 1]); 
 view(-90,0) % camera view, for some reason the point cloud always views
             % from behind so this fixes that
% rotate3d

%% Export as tif file
%view(az,el); % view for print 
% prompt = 'File name for .tif picture?\n';
% file = input(prompt,'s');
% str = [file, ".tif"];
% file_tif = join(str,"");
% % 300 dots per inc, tif image, 'file name.tif' 
% % typically 600 dpi is good for papers
% print ('-r600', '-dtiff', file_tif);
%% Rotates object to see 3d
% n = 21;
% step = linspace(-150,-50,n);
% for i=1:n
%     view(step(i),5)
%     pause (1);
% end