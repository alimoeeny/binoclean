function BuildStim(varargin)
stimno= 1;
j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'stimno',5)
        j = j+1;
        stimno = varargin{j};
    end
    j = j+1;
end


sname = ['/local/manstim/stim' num2str(stimno)];
fid = fopen(sname,'w');
vals{1} = round(rand(200,1) .*7)./70;
vals{1}(1:100) = 0.1;
vals{2}(1:100) = -1;
vals{2}(101:200) = 1;
fprintf(fid,'se%d\n',1000+stimno.*400);
fprintf(fid,'sl=0\n');
types = {'dx' 'ce'};
for k = 1:2
    fprintf(fid,'%s:',types{k});
    for j = 1:length(vals{k})
        fprintf(fid,' %.2f',vals{k}(j));
    end
    fprintf(fid,'\n');
end
