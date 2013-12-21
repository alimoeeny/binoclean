function X = BuildNsineGamma(eid, varargin)
%AllS = BuildRPExpt(...  Makes stimulus description file for binoc
%For interleaving gratings, RLS, drifinting/dynamic steppein

if nargin == 0
    eid = 1;
end
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
stimdir = sprintf('/local/expts/Gamma%d',eid);
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
    elseif strncmpi(varargin{j},'rpts',4)
        j = j+1;
        rpts = varargin{j};
    elseif strncmpi(varargin{j},'teststim',5)
        j = j+1;
        teststim = varargin{j};
    elseif strncmpi(varargin{j},'signals',5)
        j = j+1;
        stimvals{1} = varargin{j};
    end
    j = j+1;
end

stimnames = BinocStimNames;
X.stimdir = stimdir;
if eid == 1
    S.sf = 4;
    S.tf = 2;
    X.exptvars = {'st' 'co' 'sM'};
    X.explabel = 'Gamma1';
    vars{1} = {'rls' 'rls' 'nsines' 'grating'};
    vars{1} = {vars{1}{:} vars{1}{:} vars{1}{:} vars{1}{:}};
    vars{2} = [0.125 0.125 0.125 0.125 0.25 0.25 0.25 0.25 0.5 0.5 0.5 0.5 1 1 1 1];
    vars{3} = [0 5 0 0 0 5 0 0 0 5 0 0 0 5 0 0 0 5 0 0];
elseif eid == 2 %grating contrast and 2grating contrast 1 component
    S.st = 'grating';
    S.tf = 2;
    S.f2 = 1;
    S.t2 = 2;
    X.exptvars = {'st' 'co' 'c2' 'sf' 'or' 'a2'};
    X.explabel = 'Gamma2';
    stimtypes = [4 4 4 4 4 4 4 4 11 11 11 11 4]
    vars{1} = {stimnames{stimtypes}};
    vars{1}{end} = 'plaid';
    vars{2} = [0.125 0.125 0.25 0.25 0.5 0.5 1 1 1 1 1 1 1];
    vars{3} = [0 0 0 0 0 0 0 0 0.125 0.25 0.5 1 1];
    vars{4} = [4 1 4 1 4 1 4 1 4 4 4 4 4];
    vars{5} = [0 0 0 0 0 0 0 0 0 0 0 0 -45];
    vars{6} = [0 0 0 0 0 0 0 0 0 0 0 0 90];
elseif eid == 3 %size with backgr and hole
    S.st = 'grating';
    X.exptvars = {'sz' 'co' 'Bc'};
    X.explabel = 'Gamma3';
    stimtypes = [4 4 4 4 4 4 4 4 11 11 11 4]
    vars{1} = [0.5 1 1.5 2 0.5 1 1.5 2 0.5 1 1.5 2 6];
    vars{2} = [1 0 1 1 0 1 1 0 1 1 0 1 1]; %fg co
    vars{3} = [1 1 0 1 1 0 1 1 0 1 1 0 0];
end

S.nsf = [1:10];
S.dd = 100;
S.jv = 0.5;

f = fields(S);
    for j = 1:length(vars{1})
        for  k = 1:length(f)
            AllS(j).(f{k}) = S.(f{k});
        end
        for  k = 1:length(X.exptvars)
        if iscellstr(vars{k})
            AllS(j).(X.exptvars{k}) = vars{k}{j};
        else
            AllS(j).(X.exptvars{k}) = vars{k}(j);
        end
        end
    end

f = fields(AllS);
nstim = length(AllS);
for j = 1:length(AllS)
    AllS(j).stimno = j-1;
    X.stimstr{j} = WriteStim(X, AllS(j));
    fprintf('%s\n',X.stimstr{j});
end


stimorder = repmat([0:nstim-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
if teststim > 0
    cprintf('blue','Testing Stimulus %d\n',teststim);
    stimorder(1:length(stimorder)) = teststim;
else
    fprintf('%d stim x %d repeats = %d stim.\n',length(AllS),rpts,length(stimorder));
end
X.AllS = AllS;
X.stimdir = stimdir;
X.stimorder = stimorder;

fid = fopen([stimdir '/stimorder'],'w');
fprintf(fid,'exptvars=%s',X.exptvars{1});
for j = 2:length(X.exptvars)
    fprintf(fid,',%s',X.exptvars{j});
end
fprintf(fid,'\n');
fprintf(fid,'explabel=%s\n',X.explabel);
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function s = WriteStim(X, S)
stimno= 1;
j = 1;
stimnames = BinocStimNames;


sname = [X.stimdir '/stim' num2str(S.stimno)];
fid = fopen(sname,'w');

for j = 1:length(X.exptvars)
    f = X.exptvars{j};
    if ischar(S.(f))
        if strcmp(f,'st') && strcmp(S.st,'plaid')
            strs{j} = 'st=grating';
        else
            strs{j} = sprintf('%s=%s',f,S.(f));
        end
    else
        strs{j} = sprintf('%s=%.3f',f,S.(f));
    end
    fprintf(fid,'%s\n',strs{j});
end    
        if isfield(S,'st')
            S.stimid = find(strcmp(S.st,stimnames))-1;
            if strcmp(S.st,'nsines')
                fprintf(fid,'nsf=%s\n',sprintf('%.0f ',S.nsf));
                fprintf(fid,'jv=%.1f\n',S.jv);
                S.sf = S.nsf(1);
            elseif strcmp(S.st,'grating') & isfield(S,'sf')
                fprintf(fid,'sf=%.1f\n',S.sf);
                fprintf(fid,'tf=%.1f\n',S.tf);
            elseif strcmp(S.st,'2grating')
                fprintf(fid,'sf=%.1f\n',S.sf);
                fprintf(fid,'tf=%.1f\n',S.tf);
                fprintf(fid,'f2=%.1f\n',S.f2);
                fprintf(fid,'t2=%.1f\n',S.t2);
            elseif strcmp(S.st,'plaid')
                S.stimid = 3;
                fprintf(fid,'sf=%.1f\n',S.sf);
                fprintf(fid,'tf=%.1f\n',S.tf);
                fprintf(fid,'f2=%.1f\n',S.f2);
                fprintf(fid,'a2=90\n');
            elseif strcmp(S.st,'rls')
                fprintf(fid,'dd=100\n');
                fprintf(fid,'jv=%.1f\n',S.jv);
            end
        end
fprintf(fid,'exvals=');
for j = 1:length(X.exptvars)
    if strncmp(strs{j},'st=',3)
        fprintf(fid,'%d ',S.stimid);
    else
        fprintf(fid,'%s ',regexprep(strs{j},'.*=',''));
    end
end
fprintf(fid,'%d\n',S.stimno);
%.1f %.1f %.2f %.0f\n',S.tf,S.jv,S.sf,S.stimtype);
fclose(fid);
x = scanlines(sname);
for j = 1:length(x)
    x{j} = [x{j} ' '];
end
s = cat(2,x{:});


