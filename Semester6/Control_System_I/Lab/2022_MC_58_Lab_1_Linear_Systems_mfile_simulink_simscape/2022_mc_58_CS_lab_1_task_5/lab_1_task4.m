clc;
query=input('Do you want to analyze the system at single Force (5N) or on a range of forces? (Single/Range) >> ','s'); 
TR = [0 10]; 
X0 = [0;0;0;0]; 
if query=="Single" || query=="single" 
   range=[5,5]; 
   inc=1; 
elseif query=="Range" || query=="range" 
   range=input('Please enter a start and an end value for the force in the format [start,end] >> '); 
   inc=input('Please enter an increment value >> '); 
end

  for F=range(1):inc:range(2) 
    [t,y]=ode45(@(t,y) Task4Fun(t,y,F),TR,X0); 
    x1=y(:,1); 
    v1=y(:,2); 
    x2=y(:,3); 
    v2=y(:,4); 
    subplot(1,4,1); 
    plot(t,x1); 
    hold on; 
    xlabel('time'); 
    ylabel('Displacement-1'); 
    subplot(1,4,2); 
    plot(t,v1); 
    hold on; 
    xlabel('time'); 
    ylabel('Velocity-1'); 
    subplot(1,4,3); 
    plot(t,x2); 
    hold on; 
    xlabel('time'); 
    ylabel('Displacement-2'); 
    subplot(1,4,4); 
    plot(t,v2); 
    hold on; 
    xlabel('time'); 
    ylabel('Velocity-2'); 
end 
  
text=""; 
for i=range(1):inc:range(2) 
    text(end+1)=sprintf("%dN force",i); 
end 
text=text(2:end); 
for i=1:4 
    subplot(1,4,i);legend(text); 
end 