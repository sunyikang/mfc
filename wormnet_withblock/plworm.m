clear;
load wormnum.txt;
figure;
numplot=400;
plot(1:length(wormnum),wormnum);
%plot(1:numplot,worminfect(1:numplot));
title('蠕虫扩散仿真');
xlabel('t(min)');
ylabel('已染主机数');