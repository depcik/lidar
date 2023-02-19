clear
clc

Configuration=input('Run Configuration ');
switch Configuration    %0-5: config sweep, 6: cleaner C1, 7: single full cycle
    case 0
        %lidar configuration 0: Balanced Performance
        rawdata=dlmread('HFV7Lidar_36c.txt');
        low=0;
        high=8000;
        phi=0;
        name='Tree and Runoff';
        xlim([3.2,3.9]);
    case 1
        %lidar configuration 1: short range, high speed, max acquisition count
        rawdata=dlmread('convergingfencelines.txt');
        low=0;
        high=32000;
        phi=0;
        name='Converging Fencelines';
        %xlim([3.2,3.9]);
     case 2
        %lidar configuration 1: short range, high speed, max acquisition count
        rawdata=dlmread('HFV7Lidar_92c.txt');
        low=0;
        high=8000;
        phi=0;
        name='Parking Lot';
    case 3
       %lidar configuration 2: Default range, higher speed short range. Turns on quick termination detection 
       %for faster measurements at short range(with decreased accuracy)
        rawdata=dlmread('HFV7Lidar_26c.txt');
        low=0;
        high=800;
        phi=0;
        name='Pothole';
        xlim([0,1]);
    case 4
        %lidar configuration 3: Maximum range. Uses 0xff maximum acquisition count.
        rawdata=dlmread('HFV7Lidar_94c.txt');
        low=10;
        high=1000;
        phi=0;
        name='Gen 4 HFV sighting range';
    case 5
        %lidar configuration 4: High sensitivity detection. Overrides default valid measurement detection
        %algorithm, and uses a threshold value for high sensitivity and noise.
        rawdata=dlmread('HFV7Lidar_20cminus.TXT');
        low=10;
        high=1000;
        phi=-5;
        name='VW BUG';
    case 6
        %lidar configuration 5: Low sensitivity detection. Overrides default valid measurement detection
        %algorithm, and uses a threshold value for low sensitivity and noise.
        rawdata=dlmread('HFV6Lidar_27.TXT');
        low=10;
        high=1000;
        phi=-20;
        name='Configration 5';
    case 7
        rawdata=dlmread('Sighting range c11.TXT');
        low=10;
        high=1000;
        phi=-20;
        name='Gen 3 Configration 1';
    case 8
        rawdata=dlmread('LidarGen4_10.TXT');
        low=0;
        high=1000;
        phi=0;
        name='Leading edge sweep';
   
end

rawdata(rawdata(:,3)>high,:)=[];
rawdata(rawdata(:,3)<low,:)=[];
azimuth=-deg2rad(rawdata(:,1));%/.18*360/2048);
elevation=deg2rad(rawdata(:,2));%/.18*360/2048+phi);
distance=rawdata(:,3)/100;

[x,y,z]=sph2cart(azimuth,elevation,distance);
figure(1)
switch Configuration;
    case {0,1,2,3,4,5,6,7}
        scatter3(x,y,z,15,distance,'o','filled');
        %meshgrid(x,y)
   % case 6
       % scatter3(y,z,-x,10,-x,'o')
    %case {7,8}
        %scatter3(x,y,z,10,z,'o','filled')
end
xlim([0,6]);
%ylim([-11,-5]);
%zlim([-1,1])
xlabel('x-axis')
ylabel('y-axis')
zlabel('z-axis')
zlim([-1,1.5]);
%xlim([10,40]);
%zlim([zl(1)-(mean(x)-mean(z)) zl(2)-(mean(x)-mean(z))])
xlabel('X [m]');
ylabel('Y [m]');
zlabel('Z [m]');
title(name);
colormap();%colorcube);%
%caxis([2.4,5.6]);
%caxis([24,29]);
%caxis([8,9]);
%caxis([24,35]);
colorbar;
%% Depcik contourf plot
% x => this is the distance data (typically z-direction for contour plots)
% y => typically the x-direction for contour plots
% z => typically the y-direction for contour plots
%%%Xc = y; %Start here
% Yc = z;
% Zc = x;
% resX = 1000;        % How many datapoints to plot in the contour X-direction
% resY = 1000;        % How many datapoints to plot in the contour Y-direction
% resC = 10;          % How many contour lines to show
% Xi = linspace(min(Xc),max(Xc),resX);
% Yi = linspace(min(Yc),max(Yc),resY);
% Zg = griddata(Xc,Yc',Zc,Xi,Yi');
% figure(2) 
% contourf(Xi,Yi',Zg,resC);
% xlabel('Y')
% ylabel('Z')
% zlabel('X')
% colorbar %Allows for a color legends for distance 
% colormap(jet)  %Creates a more detailed colorbar.  
% 
