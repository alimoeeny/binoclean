function result = BuildExpt(varargin)
%AllS = BuildExpt(...  Makes stimulus description file for binoc
%This version does disparity/correlation subspace stimuli
%   ...,'dxvals', [dx],   give the list of dx vals for the subspace map
%   ...,'disps', [dx],   gives signal disparity values (usually one -, one +)
%   ..., 'acdisps', [ac] give disp values for the ac adaptors (first 100 frames) 
%   ..., 'signal' [x]  gives the signal strength valses (fraction of frames
%                      with signal disparity
%   ...,'ntrials', N, is the number of times each stimulus is presented
%   ...,'nrpts', R, is the number of times each exact (default 2 = twopass)

stimno= 1;
nf = 200;
j = 1;
stimvals{3} = [0];
stimvals{2} = [0];
stimvals{1} = [0];
distvals = -0.2:0.05:0.2;
ntrials = 10;
preframes = 100;
stimdir = '/local/expts/GammaPhase';
rpts = 2;

mkdir(stimdir);
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

distw = length(distvals);
S.stimno = 0;
S.sl = 0;
S.types = {'dx' 'ce'}; %these variable set frame by frame
nstim = [length(stimvals{1}) length(stimvals{2}) length(stimvals{3}) ntrials/rpts];
for t = 1:nstim(4)
for m = 1:nstim(3)
for k = 1:nstim(2)
    for j = 1:nstim(1)
        S.ph = stimvals{1}(j);
        S.backph = stimvals{2}(k);
        WriteStim(stimdir, S);        
        AllS(S.stimno+1) = S;
        S.stimno = S.stimno+1;
    end
end
end
end

stimorder = repmat([0:S.stimno-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
result.stims = AllS;
result.stimorder = stimorder;
result.stimdir = stimdir;


fid = fopen([stimdir '/stimorder'],'w');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fprintf(fid,'manexpts=sz ph Backph backor\n');
fclose(fid);

function WriteStim(stimdir, S)
stimno= 1;
j = 1;


sname = [stimdir '/stim' num2str(S.stimno)];
fid = fopen(sname,'w');
fprintf(fid,'ph=%.1f\n',S.ph);
fprintf(fid,'Backph=%.1f\n',S.backph);
fclose(fid);


