x = linspace(0, 10, 360);
t = 0:1:60;
u = zeros(size(t));
v = zeros(size(t));
w = zeros(size(t));
n =10;
for k = 1:60
    if k<=n
       u(k)=90
    elseif k>=n
        u(k)=0
    end

    if k>=n
       v(k)=90
    elseif k<=n
        v(k)=0
    end

    if k>=n+20
       w(k)=90
    elseif  k<=n+20
        w(k)=0
    end

end
a = sin(u);
b = sin(v);
c = sin(w);
subplot(3, 1, 1);
plot(t,a);
subplot(3, 1, 2);
plot(t,b);
subplot(3, 1, 3);
plot(t,c);