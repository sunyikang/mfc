clear;
load wormnum.txt;
figure;
numplot=400;
plot(1:length(wormnum),wormnum);
%plot(1:numplot,worminfect(1:numplot));
title('�����ɢ����');
xlabel('t(min)');
ylabel('��Ⱦ������');