function state_next = dyn(state,input)
%DYN Summary of this function goes here
%   Detailed explanation goes here

% transform
v = state(1);
x = state(2);
y = state(3);
theta = state(4);
w = state(5);
Tl = input(1);
Tr = input(2);
%disp(theta)

% constant parameters
ts = 0.1;
mB = 6.96; % Mass of pendulum body
mW = 0.3; % Mass of wheel
l = 0.208; % Length of pendulum
r = 0.031; % Radius of wheel
d = 0.257; % Distance between wheels(tread)
I1 = 0; I2 = 0; I3 = 4.347*10^-7; % MOI of pendulum body
K = 1.89*10^-10; J = 3.78*10^-10;  % MOI of wheel
%amax = 1; % Max linear accelerated velocity
%amaxw = 1; % Max angular accelerated velocity

% Intermedia varible
miu1 = (mB+2*mW+2*J/(r^2))*(I2+mB*l^2)-(mB^2)*(l^2);
miu2 = I3 + 2*K + 2*(mW+J/(r^2))*(d^2);
aw = (d/(r*miu2))*(Tl-Tr)*57.2958;
phi = (1/2)*aw*(ts^2) + w*ts;
A = (90 - theta + phi/2)*pi/180;
a = (((I2+mB*l^2)/r + mB*l)/miu1)*(Tl+Tr)*100;

% dyn sys
v_next = v + a*ts;
x_next = x + ((1/2)*a*ts^2 + v*ts)*cos(A);
y_next = y + ((1/2)*a*ts^2 + v*ts)*sin(A);
theta_next = theta + phi;
w_next = w + aw*ts;

state_next = [v_next;x_next;y_next;theta_next;w_next];

end

