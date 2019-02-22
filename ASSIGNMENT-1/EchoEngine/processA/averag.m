x=csvread("E.csv");
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y))