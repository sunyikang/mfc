clear;
%结点个数
NodeNum=10;
%坐标最大值
MaxCoor=200;
%读入邻接矩阵
%NodeAdj=[0 1 1 0;1 0 1 0;1 1 0 1;0 0 1 0];
load NodeAdj.txt;
%结点坐标标识矩阵
NodeCoor=zeros(MaxCoor);
%从MaxCoor×MaxCoor的坐标中选择NodeNum个点
for i=1:NodeNum
    %生成的坐标是否已被占用
    isSelected=1;
    while isSelected==1
        x=round(rand*MaxCoor);
        y=round(rand*MaxCoor);
        %在坐标系中表明该结点已被占用
        if NodeCoor(x,y)==0 
            NodeCoor(x,y)=i;
            isSelected=0;
            Node{i}=[x,y];
        end;
    end;
end;
  
%先画结点
for i=1:NodeNum
    plot(Node{i}(1),Node{i}(2),'o');
    text(Node{i}(1)+2,Node{i}(2),int2str(i-1));
    hold on;
end

%连线
for i=1:NodeNum
    for j=i+1:NodeNum
        if NodeAdj(i,j)==1
            plot([Node{i}(1),Node{j}(1)],[Node{i}(2),Node{j}(2)]);
            hold on;
        end;
    end;
end;