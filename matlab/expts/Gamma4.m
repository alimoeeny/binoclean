function AllS = Gamma2(varargin)
%Build Expt for Gamma with 2grating SF andTF sweep. RDS jx sswwp
AllS = [];

basedir = '/local/expts/Gamma4';
nr = 0;

j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'nrpt',4);
        j = j+1;
       nr = varargin{j};
    end
    j = j+1;
end

if ~exist(basedir)
    mkdir(basedir);
end
sfone = 3;
sfs = [4 3 4.0 3.0 4   4   4  4   4  4];
cos = [1 1 0.5 0.5 1   0.5 1  0.5 1  1];
ors = [0 0 0.0 0.0 -45 -45 45 45  0  0];
a2s = [0 0 0.0 0.0 0    0  0  0   90 0];
nphs =[0 0 0   0   0    0  0  0   0  360]

ncs = [0 71 0 0 9 18 24 35 71];

tfs = [0 1 2 4 8];
jxs = [0 0.02 0.04 0.08];
tf=2;
speed=0.5;

ns = 0;
stimname = [basedir '/stim' int2str(ns)];
ns = ns+1;
AllS(ns).sf = sfs(1);
fid = fopen(stimname,'w');
fprintf(fid,'st=2grating\nsf=%.1f\ntf=%.2f\n',sfs(1),tf);
fprintf(fid,'f2=%.2f\n',sfs(2));
fprintf(fid,'t2=%.2f\n',tf * sfs(2)/sfs(1));
fprintf(fid,'c2=1.0\n');
fprintf(fid,'or=0\n');
fprintf(fid,'sl=0\n');
fprintf(fid,'nc=2\n');
fprintf(fid,'Dr=1\n');

fclose(fid);

for j = 1:length(cos)
stimname = [basedir '/stim' int2str(ns)];
ns = ns+1;
AllS(ns).sf = sfs(j);
AllS(ns).co = cos(j);
fid = fopen(stimname,'w');
fprintf(fid,'st=grating\nsf=%.1f\ntf=%.2f\n',sfs(j),tf);
fprintf(fid,'co=%.2f\n',cos(j));
fprintf(fid,'f2=%.2f\n',sfs(j)); %make sure plaid is one SF
fprintf(fid,'t2=%.2f\n',tf);
fprintf(fid,'c2=1.0\n');
fprintf(fid,'sM=0\n');
fprintf(fid,'or=%.1f\n',ors(j));
fprintf(fid,'a2=%.2f\n',a2s(j));
fprintf(fid,'nph=%.2f\n',nphs(j));
fprintf(fid,'nc=1\n');
fprintf(fid,'sl=0\n');
fprintf(fid,'Dr=1\n');
fclose(fid);
end    
    

for j = 1:length(ncs)
stimname = [basedir '/stim' int2str(ns)];
ns = ns+1;
AllS(ns).nc = ncs(j);
AllS(ns).sf = 1;
fid = fopen(stimname,'w');
fprintf(fid,'st=rls\nsf=1\ntf=1\na2=0\nnph=0\nsM=29\n');
fprintf(fid,'nc=%d\n',ncs(j));
fprintf(fid,'or=0\n',ncs(j));
fprintf(fid,'co=1\n',ncs(j));
fprintf(fid,'jv=%.1f\n',speed);
if j > 3
    fprintf(fid,'sl=1\n');
    fprintf(fid,'sM=29\n');
    fprintf(fid,'Dr=1\n');
elseif j ==3
    fprintf(fid,'sl=1\n');
    fprintf(fid,'sM=30\n');
    fprintf(fid,'Dr=3\n');
else
    fprintf(fid,'sl=0\n');
    fprintf(fid,'sM=29\n');
    fprintf(fid,'Dr=1\n');
end
fclose(fid);
end

if nr <= 0
    nr = round(80./ns);
end
stimorder = repmat([1:ns]-1,1,nr);
stimorder = stimorder(randperm(length(stimorder)));
stimname = [basedir '/stimorder'];
fid = fopen(stimname,'w');
fprintf(fid,'expvars=co,a2,or,sl,nc,st,sf\n');
fprintf(fid,'%s\n',sprintf('%d ',stimorder));
fclose(fid);
fprintf('%d stim * %d repeats\n',ns,nr);