function AllS = Gamma2(varargin)
%Build Expt for Gamma with 2grating SF andTF sweep. RDS jx sswwp
AllS = [];

basedir = '/local/expts/Gamma4';
sfone = 3;
sfs = [4 3 4.0 3.0 4   4   4  4   4  4];
cos = [1 1 0.5 0.5 1   0.5 1  0.5 1  1];
ors = [0 0 0.0 0.0 -45 -45 45 45 -45 0];
a2s = [0 0 0.0 0.0 0    0  0  0   90 0];
nphs =[0 0 0   0   0    0  0  0   0  360]

ncs = [1 9 13 25 31];

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
fprintf(fid,'t2=%.2f\n',tf);
fprintf(fid,'c2=1.0\n');
fprintf(fid,'or=0\n');
fclose(fid);

for j = 1:length(cos)
stimname = [basedir '/stim' int2str(ns)];
ns = ns+1;
AllS(ns).sf = sfs(j);
AllS(ns).co = cos(j);
fid = fopen(stimname,'w');
fprintf(fid,'st=grating\nsf=%.1f\ntf=%.2f\n',sfs(j),tf);
fprintf(fid,'co=%.2f\n',cos(j));
fprintf(fid,'t2=%.2f\n',tf);
fprintf(fid,'c2=1.0\n');
fprintf(fid,'or=%.1f\n',ors(j));
fprintf(fid,'a2=%.2f\n',a2s(j));
fprintf(fid,'nph=%.2f\n',nphs(j));
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
fprintf(fid,'jv=%.1f\n',speed);
fclose(fid);
end


nr = round(80./ns);
stimorder = repmat([1:ns]-1,1,nr);
stimorder = stimorder(randperm(length(stimorder)));
stimname = [basedir '/stimorder'];
fid = fopen(stimname,'w');
fprintf(fid,'expvars co,a2,or,sl,nc\n');
fprintf(fid,'%s\n',sprintf('%d ',stimorder));
fclose(fid);
fprintf('%d stim * %d repeats\n',ns,nr);