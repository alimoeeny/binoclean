function X = BuildNsineGamma(varargin)
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
teststim = 0;
preframes = 100;
rpts = 3;
stimdir = '/local/expts/NSineGamma2';
if ~exist(stimdir)
    mkdir(stimdir);
end

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
    end
    j = j+1;
end

X.exptvars = {'tf' 'sf' 'st' 'nsf' 'jv'};


n = 1;
speed= [0.5 1 2];
tfs = [1 2 4];
AllS(n).st = 'nsines';
AllS(n).stimtype = 19;

AllS(n).tf=tfs(1);
AllS(n).jv=0;
AllS(n).sl=100;
AllS(n).nsf = [1 2 4];

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=tfs(2);

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=tfs(3);

n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv=speed(1);
n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv=speed(2);
n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv=speed(3);

n = n+1; AllS(n) = AllS(n-1);
AllS(n).nsf = [1:10];
AllS(n).jv = 2;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).nsf = [1:10];
AllS(n).jv = 0;
AllS(n).tf = 0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).st = 'rls';
AllS(n).stimtype = 16;
AllS(n).jv = 0;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv = 2;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).nsf = [2 4];
AllS(n).jv = 0;
AllS(n).tf = 4;



n = n+1; AllS(n) = AllS(n-1);
AllS(n).st = 'grating';
AllS(n).stimtype = 3;
AllS(n).sf = 1;
AllS(n).jv=0;
AllS(n).tf=1;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=2;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=4;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=8;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).sf = [2];
AllS(n).tf=1;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=2;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=4;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=8;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=0;


n = n+1; AllS(n) = AllS(n-1);
AllS(n).st = 'grating';
AllS(n).stimtype = 3;
AllS(n).sf = [4];
AllS(n).jv=0;
AllS(n).tf=1;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=2;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=4;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=8;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf=0;

f = fields(AllS);
nstim = length(AllS);
for j = 1:length(AllS)
    AllS(j).stimno = j-1;
    X.stimstr{j} = WriteStim(stimdir, AllS(j));
    fprintf('%s\n',X.stimstr{j});
end


stimorder = repmat([0:nstim-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
if teststim > 0
    stimorder(1:length(stimorder)) = teststim;
end
X.AllS = AllS;
X.stimdir = stimdir;
X.stimorder = stimorder;

fid = fopen([stimdir '/stimorder'],'w');
fprintf(fid,'manexp=tf+sf+jv+st+nsf\n');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function s = WriteStim(stimdir, S)
stimno= 1;
j = 1;


sname = [stimdir '/stim' num2str(S.stimno)];
fid = fopen(sname,'w');

fprintf(fid,'st=%s\n',S.st);
fprintf(fid,'tf=%.1f\n',S.tf);
if strcmp(S.st,'nsines')
    fprintf(fid,'nsf=%s\n',sprintf('%.0f ',S.nsf));
    fprintf(fid,'jv=%.1f\n',S.jv);
    S.sf = S.nsf(1);
elseif strcmp(S.st,'grating')
    fprintf(fid,'sf=%.1f\n',S.sf);
elseif strcmp(S.st,'rls')
    fprintf(fid,'dd=100\n');
    fprintf(fid,'jv=%.1f\n',S.jv);
else
    fprintf(fid,'jv=%.1f\n',S.jv);

end
    
    
fprintf(fid,'exvals=%.1f %.1f %.2f %.0f\n',S.tf,S.jv,S.sf,S.stimtype);
fclose(fid);
x = scanlines(sname);
for j = 1:length(x)
    x{j} = [x{j} ' '];
end
s = cat(2,x{:});


