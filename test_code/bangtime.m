%% This file calculates the bang bang times for each of the axes%%
%% Note that the final velocity is assumed to be 0%%
function [T,u]=bangtime(z0,zF,v0)
C=z0+v0-zF;
eC=exp(C);
if v0 >= 0
   if eC <= 1+v0
      u=1;
   else
      u=-1;
   end
else
   if eC <= 1/(1-v0)
      u=1;
   else
      u=-1
   end
end
t2=log(1+sqrt(1+exp(C/u)*((v0/u)-1)));
t1=t2-C/u;
T=[t1;t2];
%% So the total time taken is t1+t2%%    