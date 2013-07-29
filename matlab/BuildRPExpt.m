function varargout = BuildRPExpt(varargin)
%AllS = BuildRPExpt(...  Makes stimulus description file for binoc
%For interleaving gratings, RLS, drifinting/dynamic steppein
stimno= 1;
nf = 200;
j = 1;
stimvals{3} = [-0.1 0.1];
stimvals{2} = [0 1]; %random or drifting
stimvals{1} = [0 1 2 4 8.3 16.7]; %tf
distvals = -0.2:0.05:0.2;
ntrials = 10;
preframes = 100;
rpts = 1;

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



n = 1;
AllS(n).st = 'grating';
AllS(n).tf = 0;
AllS(n).sl=0;
AllS(n).nph = 360;
AllS(n).op='-cr';

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf = 2;
AllS(n).sl= 25;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).nph = 0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf = 2.08333;
AllS(n).op='-cr';
AllS(n).sl=6;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=12;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=25;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=0;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='+cr';


n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf = 4.1666;
AllS(n).op='-cr';
AllS(n).sl=3;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=6;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=12;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=0;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='+cr';

n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='-cr';
AllS(n).tf = 8.333;
AllS(n).sl=3;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=6;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=0;
n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='+cr';


n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='-cr';
AllS(n).st = 'rls';
AllS(n).jv = 0.502;
AllS(n).sl=1;
AllS(n).nph = 0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv = 1.04;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).jv = 2.08;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).sl=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).tf = 4.1666;
AllS(n).sl=1;
AllS(n).op='+cr';
AllS(n).jv=0;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='-cr';
AllS(n).sl=24;

n = n+1; AllS(n) = AllS(n-1);
AllS(n).op='+cr';
AllS(n).st = 'rds';
AllS(n).tf = 4.1666;
AllS(n).sl=1;
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
    WriteStim(AllS(j));
end


stimorder = repmat([0:nstim-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
stimorder(2:2:end) = 1; %for testing this stim
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
fprintf(fid,'st=%s\n',S.st);
fprintf(fid,'nph=%d\n',S.nph);
fprintf(fid,'tf=%.1f\n',S.tf);
if strcmp(S.st,'rls')
    fprintf(fid,'jv=%.1f\n',S.jv);
end
if strcmp(S.st,'rds')
    fprintf(fid,'dd=100\njv=%.1f\n',S.jv);
end
fprintf(fid,'sl=%d\n',S.sl);
fprintf(fid,'op=%s\n',S.op);
fprintf(fid,'exvals%.2f %.2f %.2f\n',S.tf,S.nph,S.sl);
fclose(fid);


