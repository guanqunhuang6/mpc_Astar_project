%clear;
% read path
path = importdata('path.txt');
path(:,1) = 0.3*path(:,1);
path(:,2) = 0.3*path(:,2);
%new_path = path(1:5:1206,:);
%path = new_path;

% PQR
P = [0 0 0 0 0; 0 30 0 0 0; 0 0 30 0 0; zeros(2,5)];
Q = [0 0 0 0 0; 0 100 0 0 0; 0 0 100 0 0; zeros(2,5)]; % good is 10*p
R = 0.01*eye(2); % a and w use 0.01
N = 5;

% current state
x0 = [0;0;0;45;0];
xL = [0; 0; 0; -3600; -30];
xU = [10; 10000; 10000; 3600; 30];
x_real = zeros(5,1201);
x_real(:,1) = [0;0;0;45;0];
u_real = zeros(2,1201);
u_real(:,1) = [0;0];
ite = 0;

% begin run
while(1)
    ite = ite + 1;
    fprintf('Solving simstep: %i\n',ite);
    row = find_in_path(x_real(2,ite), x_real(3,ite),path);
    try
        pointset = path(row:15:row+74,:);
    catch ME
        break;
    end
    [feas, xOpt, uOpt, sol] = solve_cftoc3(P, Q, R, N, x_real(:,ite), u_real(:,ite), xL, xU, pointset);
    x_real(:,ite+1) = xOpt(:,2);
    u_real(:,ite+1) = uOpt(:,2);
end

% new PQR
P = [10 0 0 0 0; 0 30 0 0 0; 0 0 30 0 0; 0 0 0 10 0; 0 0 0 0 10];
Q = 2.0*P;
R = 0.01*eye(2); % a and w use 0.01
N = 5;
ite = ite - 1;
pointset = path(row:int32((length(path)-row)/4):length(path),:);
pointset = [pointset; path(length(path),:)];

for i = 1:20
    ite = ite + 1;
    fprintf('Solving simstep: %i\n',ite);
    [feas, xOpt, uOpt, sol] = solve_cftoc2(P, Q, R, N, x_real(:,ite), u_real(:,ite), xL, xU, pointset);
    x_real(:,ite+1) = xOpt(:,2);
    u_real(:,ite+1) = uOpt(:,2);
end
% 
%%
% 500
obstacle1 = Polyhedron('lb',[142.5;142.5],'ub',[157.5;157.5]);
% 600
obstacle2 = Polyhedron('lb',[172.5;172.5],'ub',[187.5;187.5]);
% 700
obstacle3 = Polyhedron('lb',[202.5;202.5],'ub',[217.5;217.5]);
% 800
obstacle4 = Polyhedron('lb',[232.5;232.5],'ub',[247.5;247.5]);
% 900
obstacle5 = Polyhedron('lb',[262.5;262.5],'ub',[277.5;277.5]);

path = importdata('path.txt');

figure(3)
plot(obstacle1);
hold on
plot(turtlebotsize1,'color','green','alpha',0.1)
hold on
plot(noise1,'color','blue','alpha',0.1)
hold on
plot(obstacle2);
hold on
plot(turtlebotsize2,'color','green','alpha',0.1)
hold on
plot(noise2,'color','blue','alpha',0.1)
hold on
plot(obstacle3);
hold on
plot(turtlebotsize3,'color','green','alpha',0.1)
hold on
plot(noise3,'color','blue','alpha',0.1)
hold on
plot(obstacle4);
hold on
plot(turtlebotsize4,'color','green','alpha',0.1)
hold on
plot(noise4,'color','blue','alpha',0.1)
hold on
plot(obstacle5);
hold on
plot(turtlebotsize5,'color','green','alpha',0.1)
hold on
plot(noise5,'color','blue','alpha',0.1)
hold on
%%
% xL = [0; 0; 0; -360; -360];
% xU = [10; 100; 100; 360; 360];
% % [feas, xOpt, uOpt] = solve_cftoc(P, Q, R, N, x0, xL, xU, pointset);
t = 500;
plot(path(:,1),path(:,2),'k');
hold on
plot(x_real(2,1:5:t),x_real(3,1:5:t),'.')
hold on
plot(x_real_simu(2,1:5:t),x_real_simu(3,1:5:t),'b.')
axis([0 360 0 360])
hold on
plot(obstacle1);
hold on
plot(obstacle2);
hold on
plot(obstacle3);
hold on
plot(obstacle4);
hold on
plot(obstacle5);
legend('Planned Path','Matlab_imulation','Turtlebot_trajectory',...
    'obstacle1','obstacle2','obstacle3','obstacle4','obstacle5');
%% write
speeds = x_real(1,1:457);
angulars = x_real(5,1:457);
fid=fopen(['speeds.txt'],'w');%??????           % ??????????
 for i=1:457
  fprintf(fid,'%f\t',speeds(i));
  fprintf(fid,'%f\t',angulars(i));
  fprintf(fid,'\r\n');
 end
fclose(fid);

%% input
subplot(2,1,1);
plot(1:500,u_real(1,1:500));
hold on
plot(1:500,u_real(2,1:500));
legend('Left Wheel torque','Right Wheel Torque');
title('Matlab simulation')
subplot(2,1,2);
plot(1:500,u_real_simu(1,1:500));
hold on
plot(1:500,u_real_simu(2,1:500));
legend('Left Wheel torque','Right Wheel Torque');
title('Turtlebot')