delays=[];

x=csvread("A.csv");
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(1)=avg;

x=csvread("B.csv");
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(2)=avg;

x=csvread("C.csv");
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(3)=avg;

x=csvread("D.csv");
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(4)=avg;

x=csvread("E.csv");
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(5)=avg;