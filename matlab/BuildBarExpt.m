function varargout = BuildExpt(varargin)
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
stimvals{3} = [-0.1 0.1];
stimvals{2} = [-0.1 0.1];
stimvals{1} = [0 0.2 0.4 0.8];
distvals = -0.2:0.05:0.2;
ntrials = 10;
preframes = 100;
rpts = 2;

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
S.types = {'bar1xo' 'bar2xo' 'bar2yo' 'bar2co'}; %these variable set frame by frame
nstim = [length(stimvals{1}) length(stimvals{2}) length(stimvals{3}) ntrials/rpts];
for t = 1:nstim(4)
for m = 1:nstim(3)
for k = 1:nstim(2)
    for j = 1:nstim(1)
        nsig = floor((stimvals{1}(j) .* (nf-preframes)));
        S.vals{1} = distvals(ceil(rand(nf+1,1) .*distw));
        S.vals{2} = distvals(ceil(rand(nf+1,1) .*distw));
        S.vals{3} = distvals(ceil(rand(nf+1,1) .*distw));
        S.vals{4} = round(rand(nf+1,1));
        S.ac = stimvals{2}(k);
        S.Dc = stimvals{1}(j);
        S.se = 1000 + S.stimno .*200;
        S.dx = stimvals{3}(m);
        S.signal = stimvals{1}(j) .* sign(S.dx);
        WriteStim(S);
        
        AllS(S.stimno+1) = S;
        S.stimno = S.stimno+1;
    end
end
end
end

stimorder = repmat([0:S.stimno-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
if nargout > 0
    varargout{1} = AllS;
end
if nargout > 1
    varargout{2} = stimorder;
end

fid = fopen('/local/manstim/stimorder','w');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function WriteStim(S)
stimno= 1;
j = 1;


sname = ['/local/manstim/stim' num2str(S.stimno)];
fid = fopen(sname,'w');
fprintf(fid,'psyv=%.4f\n',S.signal);
fprintf(fid,'se=%d\n',S.se);
fprintf(fid,'exvals%.2f %.2f %.2f\n',S.dx,S.Dc,S.ac);
fprintf(fid,'dx=%0.2f\n',S.dx);
fprintf(fid,'ac=%0.2f\n',S.ac);
fprintf(fid,'sl=%d\n',S.sl);
fprintf(fid,'nbars=2\n');
types = {'dx' 'ce'};
for k = 1:length(S.types)
    fprintf(fid,'%s:',S.types{k});
    for j = 1:length(S.vals{k})
        fprintf(fid,' %.2f',S.vals{k}(j));
    end
    fprintf(fid,'\n');
end
fclose(fid);


