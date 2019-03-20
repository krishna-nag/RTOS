delays=[];

x=csvread('pA.csv');
x=x(:,1);
y=x(x>1000000);
x(x>1000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(1)=avg;

x=csvread('A2.csv');
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(2)=avg;

x=csvread('A3.csv');
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(3)=avg;

x=csvread('A4.csv');
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(4)=avg;

x=csvread('A5.csv');
x=x(:,1);
y=x(x>10000000);
x(x>10000000)=0;
avg=sum(x)/(length(x)-length(y));
delays(5)=avg;

delays