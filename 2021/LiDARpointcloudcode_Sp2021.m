clc; close all; clear;

[x y z]=xyzread('output.xyz');  %Read .xyz files and separates into X,Y, and Z components.  

for i = 1: length(x)  %For loops allows getting rid of points, shifting rows, and setting domain ranges.  

    if x(i) > 350    %Setting Distance in which points at and after will be deleted  

        x(i) = NaN;  %Nullifies the point 

    end 
end
for i = 1: length(x)  %For loops allows getting rid of points, shifting rows, and setting domain ranges.  

    if x(i) < 5    %Setting Distance in which points at and after will be deleted  

        x(i) = NaN;  %Nullifies the point 

    end 
end
% %     if mod(z(i),2) == 0 %This line looks at shifted rows 
% % 
% %         y(i) =y(i) +6;  %Shifts rows according to number 
% % 
%      end 
% 
%     if y(i)<=6 || y(i) >= 164 %Sets parameters for domain range  
% 
%         x(i)=NaN;       %Nullifies points 
% 
%     end 
% 
% end 

%% Plotting 3D Scatter plot from data run

% 3-D View of plot
figure(1) 
scatter3(x,y,z,25,x,'filled'); %Plots points into a 3D scatter.  
xlabel('X')
ylabel('Y')
zlabel('Z')
view(-135,35)   %Sets orientation of the graph 

colorbar %Allows for a color legends for distance 
colormap(jet)  %Creates a more detailed colorbar.  


% 2-D View of plot
figure(2) 
scatter3(x,y,z,25,x,'filled'); %Plots points into a 3D scatter.  
xlabel('X')
ylabel('Y')
zlabel('Z')
view(90,-1)   %Sets orientation of the graph 
colorbar %Allows for a color legends for distance 
colormap(jet)  %Creates a more detailed colorbar.  


%% Backup 3D scatter plot coding, not correct color coordinates
% figure(2) 
% 
% ptCloud= pointCloud([x y z]); %Plots points into a 3D scatter.  
% 
% view(-90,-1)   %Sets orientation of the graph 
% 
% colorbar %Allows for a color legends for distance 
% 
% colormap(jet) %Creates a more detailed colorbar.  
% 
% pcshow(ptCloud)
% 
% xlabel('x')
% ylabel('y')
% zlabel('z')



%% This section's code allows multiple .xyz files to be read and plotted all at once. 

%Could not be needed but maybe helpful in certain situtations  

%  clc;clear all; close all 

 

% xyzfiles = dir('*.xyz') ; 
% 
% N = length(xyzfiles) ; 
% 
% for i = 1:N 
% 
%      thisfile = xyzfiles(i).name  
% 
%      [x,y,z]=xyzread(thisfile); 
% 
%      figure(i) 
% 
%      scatter3(x,y,z,30,x,'filled') 
% 
%     colorbar 
% 
%     colormap(jet) 
% 
%     view(-90,-1) 
% 
% end 

%% Depcik contourf plot
% x => this is the distance data (typically z-direction for contour plots)
% y => typically the x-direction for contour plots
% z => typically the y-direction for contour plots
Xc = y;
Yc = z;
Zc = x;
resX = 1000;        % How many datapoints to plot in the contour X-direction
resY = 1000;        % How many datapoints to plot in the contour Y-direction
resC = 10;          % How many contour lines to show
Xi = linspace(min(Xc),max(Xc),resX);
Yi = linspace(min(Yc),max(Yc),resY);
Zg = griddata(Xc,Yc',Zc,Xi,Yi');
figure(3) 
contourf(Xi,Yi',Zg,resC);
xlabel('Y')
ylabel('Z')
zlabel('X')
colorbar %Allows for a color legends for distance 
colormap(jet)  %Creates a more detailed colorbar.  