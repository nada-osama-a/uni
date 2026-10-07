clear all ;close all;clc;

nneg=-15:-1;
npos=1:15;
%for Dn negative numbers
fnneg=zeros(1,length(nneg));
for k=1:length(nneg)
    n_val=nneg(k);
    if mod(abs(n_val),2)==1  %check if odd
        fnneg(k)=(2/((n_val.^2).*pi.^2))-1j/(n_val*pi);
    else
        %even
        fnneg(k)=1j/(n_val*pi);
    end
end

fnpos=zeros(1,length(npos));
for k=1:length(npos)
    n_val=npos(k);
   if mod(abs(n_val),2)==1  %check if odd
       fnpos(k)=(2/((n_val.^2).*pi.^2))-1j/(n_val*pi);
   else
       fnpos(k)=1j/(n_val*pi);
   end
end

f0=1.5;
%combine all
n=[nneg,0,npos];
Fn=[fnneg,f0,fnpos];

%plotting amplitude spectrum
figure;
subplot(2,2,1);
stem(n,abs(Fn),"filled",'LineWidth',1.5,'MarkerSize',6);
title('Amplitude Spectrum |Dn|');
xlabel('n-harmonic number');
ylabel('|Dn|');
grid on;
%plotting phase spectrum
subplot(2,2,2);
stem(n,angle(Fn),"filled",'LineWidth',1.5,'MarkerSize',6);
title('Phase Spectrum <Dn radians');
xlabel('n-harmonic number');
ylabel('angle Dn (rad)');
grid on;

%reconstraction (t from -2 to 2)
t= -4:0.01:8;%3 period
k=0;
%fapprox=zeros(1,length(t));
for t_val=t
    k=k+1;
    fapprox(k)=sum(Fn.*exp(1j*n*(pi/2)*t_val));
end

% Original periodic signal
fexact = zeros(1, length(t));
for k = 1:length(t)
    t_mod = mod(t(k) + 2, 4) - 2;
    if t_mod < 0
        fexact(k) = t_mod + 2;
    else
        fexact(k) = 2;
    end
end


subplot(2,2,3)
plot(t,(fapprox),'b-','LineWidth',1.5);
title('Reconstructed signal');
xlabel('t');
ylabel('x(t)');
grid on

subplot(2,2,4)
plot(t,fexact,'r-','LineWidth',1.5);
title('Original Signal');
xlabel('t')
ylabel('x(t);');
grid on