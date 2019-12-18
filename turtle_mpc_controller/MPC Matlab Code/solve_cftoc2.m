function [feas, xOpt, uOpt, sol] = solve_cftoc4(P, Q, R, N, x0, u0, xL, xU, pointset)

yalmip('clear')
nx = 5;
nu = 2;
x = sdpvar(nx,N+1);
u = sdpvar(nu,N+1);

points = zeros(5,N+1);
points(:,1) = x0;
% pointset
for i = 2:N+1
    points(:,i) = [0;pointset(i-1,1);pointset(i-1,2);90;0];
end

feas = false;

constr = [x(:,1) == x0 u(:,1) == u0];

cost = (x(:,N+1)-points(:,N+1))'*P*(x(:,N+1)-points(:,N+1));

for k = 1:N
    constr = [constr, x(:,k+1) == dyn(x(:,k),u(:,k+1)), ...
        x(1,k+1) <= 10, x(5,k+1)<= 30, -30 <= x(5,k+1), ...
        u(1,k+1) <= 1, -1 <= u(1,k+1), -1 <= u(2,k+1), ...
        u(2,k+1) <= 1 ];
    cost = cost + (x(:,k)-points(:,N+1))'*Q*(x(:,k)-points(:,N+1)) + ...
        (u(:,k+1)-u(:,k))'*R*(u(:,k+1)-u(:,k));
end

%     options = sdpsettings('verbose',0);
options = sdpsettings('verbose',0,'solver','fmincon');
sol = optimize(constr,cost,options);

if sol.problem == 0
    feas = true;
else
    xOpt = [];
    uOpt = [];
    return;
end

xOpt = double(x);
uOpt = double(u);