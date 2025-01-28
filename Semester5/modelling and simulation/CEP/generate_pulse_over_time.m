x = linspace(0, 10, 360);
t = 0:1:30;
u = zeros(size(t));
v = zeros(size(t));
w = zeros(size(t));
n =10;

    for l1 = 1:n
        u(l1) = 90
        v(l1)= 0
        w(l1)=0
    end

    for l2 = n:n+n
        u(l2) = 0;
        v(l2) = 90;
        w(l2) = 0;
    end

    for l3 = n+n:n+n+n
        u(l3) = 0;
        v(l3) = 0;
        w(l3) = 90;
    end

a = sin(u);
b = sin(v);
c = sin(w);
subplot(3, 1, 1);
plot(t,a);
xlabel('Time (ms)');
ylabel('Signal A');
subplot(3, 1, 2);
plot(t,b);
xlabel('Time (ms)');
ylabel('Signal B');
subplot(3, 1, 3);
plot(t,c);
xlabel('Time (ms)');
ylabel('Signal C');
ylim([-0.1 1.1]);