function varargout = BuildNsineGamma(varargin)
%AllS = BuildRPExpt(...  Makes stimulus description file for binoc
%For interleaving gratings, RLS, drifinting/dynamic steppein
stimno= 1;
nf = 200;
j = 1;
stimvals{2} = [0 1 2]; %foreground alone, background dynamic, foreground dynamic
lables = {'ForeAlone' 'BackDyn' 'ForeDyn'};
stimvals{1} = [0 0.25 0.5 0.75 1 1.25 1.5 2 3]; %sizes
ntrials = 1;
preframes = 100;
rpts = 3;
basedir = '/local/expts/GammaSize';

while j <= length(varargin)
    if strncmpi(varargin{j},'stimno',5)
        j = j+1;
        stimno = varargin{j};
    elseif strncmpi(varargin{j},'ntrials',5)
        j = j+1;
        ntrials = varargin{j};
    elseif strncmpi(varargin{j},'nrpts',5)
        j = j+1;
        rpts = varargin{j};
    end
    j = j+1;
end

n = 1;
for j = 1:length(stimvals{1})
    for k = 1:length(stimvals{2}
        AllS(n).sz = stimvals{1}(j);
        if stimvals{2}(k) == 0 %foreground, drifting, alone
            AllS(n).backstim.co = 0;
            AllS(n).sl = 1;
        elseif stimvals{2}(k) == 1 %foreground, drifting, backgr dynamic
            AllS(n).backstim.co = 1;
            AllS(n).backstim.sl=0;
            AllS(n).sl = 1;
        elseif stimvals{2}(k) == 2 %foreground dynamic, backgr drifts
            AllS(n).backstim.co = 1;
            AllS(n).backstim.sl=1;
            AllS(n).sl = 0;
        end
        AllS(n).btype = k;
    end
    n = n+1;
end


for j = 1:length(AllS)
    S = AllS(j);
    sname = [baseidr '/stim' num2str(S.stimno)];
    fid = fopen(sname,'w');

    fprintf(fid,'mo=back');
    fprintf(fid,'co=%.2f',S.backstim.co);
    fprintf(fid,'sl=%.2f',S.backstim.sl);
    fprintf(fid,'mo=fore');
    fprintf(fid,'sl=%.2f',S.sl);
    fprintf(fid,'sl=%.2f',S.backstim.sl;
    fprintf(fid,'exvals%.2f %.2f %.2f %d\n',S.sz,S.backstim.sl,S.sl,j);
    fprintf(fid,'stimtag=%s',labels{S.btype})
    fclose(fid);
end




stimorder = repmat([0:nstim-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
if nargout > 0
    varargout{1} = AllS;
end
if nargout > 1
    varargout{2} = stimorder;
end

fid = fopen([basedir '/stimorder'],'w');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function WriteStim(S)
stimno= 1;
j = 1;


