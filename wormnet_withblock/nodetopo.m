clear;
%������
NodeNum=10;
%�������ֵ
MaxCoor=200;
%�����ڽӾ���
%NodeAdj=[0 1 1 0;1 0 1 0;1 1 0 1;0 0 1 0];
load NodeAdj.txt;
%��������ʶ����
NodeCoor=zeros(MaxCoor);
%��MaxCoor��MaxCoor��������ѡ��NodeNum����
for i=1:NodeNum
    %���ɵ������Ƿ��ѱ�ռ��
    isSelected=1;
    while isSelected==1
        x=round(rand*MaxCoor);
        y=round(rand*MaxCoor);
        %������ϵ�б����ý���ѱ�ռ��
        if NodeCoor(x,y)==0 
            NodeCoor(x,y)=i;
            isSelected=0;
            Node{i}=[x,y];
        end;
    end;
end;
  
%�Ȼ����
for i=1:NodeNum
    plot(Node{i}(1),Node{i}(2),'o');
    text(Node{i}(1)+2,Node{i}(2),int2str(i-1));
    hold on;
end

%����
for i=1:NodeNum
    for j=i+1:NodeNum
        if NodeAdj(i,j)==1
            plot([Node{i}(1),Node{j}(1)],[Node{i}(2),Node{j}(2)]);
            hold on;
        end;
    end;
end;