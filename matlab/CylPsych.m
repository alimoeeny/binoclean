function X = CylPsych(varargin)
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
stimdir = '/local/expts/Cylinder';
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
dxs = [-0.05 -0.03 -0.01 0 0.01 003 0.05];

for n = 1:length(dxs)
    AllS(n).dx=dxs(n);
    AllS(n).ce=1;
    AllS(n).sa=3;
    AllS(n).st = 'cylinder';
    AllS(n).op = '+sq+afc';
    
end
n = n+1;
AllS(n).ce=0;
AllS(n).sa=0;
AllS(n).op= '-sq-afc';
AllS(n).st = 'rds';

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
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function s = WriteStim(stimdir, S)
stimno= 1;
j = 1;


sname = [stimdir '/stim' num2str(S.stimno)];
fid = fopen(sname,'w');

fprintf(fid,'dx=%.4f\n',S.dx);
fprintf(fid,'ce=%.4f\n',S.ce);
fprintf(fid,'op=%s\n',S.op);
fprintf(fid,'st=%s\n',S.st);
fprintf(fid,'sa=%.4f\n',S.sa);
    
fprintf(fid,'exvals%.1f %.0f\n',S.dx,S.stimno);
fclose(fid);
x = scanlines(sname);
for j = 1:length(x)
    x{j} = [x{j} ' '];
end
s = cat(2,x{:});


