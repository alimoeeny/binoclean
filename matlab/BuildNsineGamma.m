function result = BuildNsineGamma(varargin)
%AllS = BuildRPExpt(...  Makes stimulus description file for binoc
%For interleaving gratings, RLS, drifinting/dynamic steppein
stimno= 1;
nf = 200;
j = 1;
stimvals{3} = [-0.1 0.1];
stimvals{2} = [0 1]; %random or drifting
stimvals{1} = [0 1 2 4 8.3 16.7]; %tf
distvals = -0.2:0.05:0.2;
ntrials = 1;
teststim = 8;
preframes = 100;
teststim = 8;
rpts = 8;
stimdir = '/local/expts/NsineGamma';


while j <= length(varargin)
    if strncmpi(varargin{j},'stimno',5)
        j = j+1;
        stimno = varargin{j};
    elseif strncmpi(varargin{j},'dxvals',5)
        j = j+1;
        distvals = varargin{j};
    elseif strncmpi(varargin{j},'disps',5)
        j = j+1;
        stimvals{2} = varargin{j};
    elseif strncmpi(varargin{j},'ntrials',5)
        j = j+1;
        ntrials = varargin{j};
    elseif strncmpi(varargin{j},'nrpts',5)
        j = j+1;
        rpts = varargin{j};
    elseif strncmpi(varargin{j},'acdisps',5)
        j = j+1;
        stimvals{3} = varargin{j};
    elseif strncmpi(varargin{j},'signals',5)
        j = j+1;
        stimvals{1} = varargin{j};
    elseif strncmpi(varargin{j},'teststim',7)
        j = j+1;
        teststim = varargin{j};
    end
    j = j+1;
end

mkdir(stimdir);

n = 1;
speed=5;
AllS(n).tf=0.1;
AllS(n).jv=speed;
AllS(n).sl=50;
AllS(n).cjump = [1 1 1];

n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [1 0 0];
AllS(n).jv=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [1 0 0];
AllS(n).jv=speed;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [0 1 0];
AllS(n).jv=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [0 1 0];
AllS(n).jv=speed;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [0 0 1];
AllS(n).jv=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [0 0 1];
AllS(n).jv=speed;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [0 0 0];
AllS(n).sl = 0;
AllS(n).jv=speed;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).cjump = [0 0 0];
AllS(n).jv=0;

f = fields(AllS);
nstim = length(AllS);
for j = 1:length(AllS)
    for k = 1:length(f)
        if isnumeric(AllS(j).(f{k}))
            fprintf('%s=%.2f ',f{k},AllS(j).(f{k}));
        else
            fprintf('%s=%s ',f{k},AllS(j).(f{k}));
        end
    end
    fprintf('\n');
    AllS(j).stimno = j-1;
    WriteStim(stimdir, AllS(j));
end

if teststim
    stimorder = repmat(teststim,rpts);
else
    stimorder = repmat([0:nstim-1],rpts);
end
stimorder = stimorder(randperm(length(stimorder)));
result.stims = AllS;
result.stimdir = stimdir;
fid = fopen([stimdir '/stimorder'],'w');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function WriteStim(stimdir, S)
stimno= 1;
j = 1;


sname = [stimdir '/stim' num2str(S.stimno)];
fid = fopen(sname,'w');

fprintf(fid,'tf=%.1f\n',S.tf);
fprintf(fid,'jv=%.1f\n',S.jv);
fprintf(fid,'sl=%d\n',S.sl);
if S.sl == 0
    fprintf(fid,'nph=0\n');
else
    fprintf(fid,'nph=360\n');
end
    
fprintf(fid,'jcomp=%s\n',sprintf('%d ',S.cjump));
fprintf(fid,'exvals%.2f %.2f %.2f\n',S.tf,S.jv,S.sl);
fclose(fid);


