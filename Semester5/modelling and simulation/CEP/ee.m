v = 12;
i = zeros(1,3000);
r = 0.090;
l = 0.01;
t = 50 ;
w = zeros(1,3000);
for k = 1:3000
    w(k) = (i(k)*i(k)*(r+l))/(1.73*t);

end
plot(i,w)