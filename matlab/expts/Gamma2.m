function AllS = Gamma2(varargin)
%Build Expt for Gamma with 2grating SF andTF sweep. RDS jx sswwp
AllS = [];

basedir = '/local/expts/Gamma2';
sfone = 3;
sfs = [0.5 1 2 4 8];
tfs = [0 1 2 4 8];
jxs = [0 0.02 0.04 0.08];
cos = [1 0.5 0.25];
tf=2;
speed=0.5;

ns = 0;
for j = 1:length(sfs)
    stimname = [basedir '/stim' int2str(ns)];
    ns = ns+1;
    AllS(ns).sf = sfs(j);
    fid = fopen(stimname,'w');
    fprintf(fid,'st=2grating\nsf=%.1f\ntf=%.2f\n',sfone,tf);
    fprintf(fid,'f2=%.2f\n',sfs(j));
    fprintf(fid,'t2=%.2f\n',tf);
    fprintf(fid,'c2=1.0\n');
    fprintf(fid,'nph=0\n');
    fclose(fid);
end
for j = 1:length(cos)
stimname = [basedir '/stim' int2str(ns)];
ns = ns+1;
AllS(ns).sf = sfs(j);
fid = fopen(stimname,'w');
fprintf(fid,'st=2grating\nsf=4\ntf=%.2f\n',sfone,tf);
fprintf(fid,'f2=3\n',sfs(j));
fprintf(fid,'op=+rr\nnph=360\n');
fprintf(fid,'c2=%.2f\n',cos(j));
fclose(fid);
end

for j = 1:length(tfs)
    stimname = [basedir '/stim' int2str(ns)];
    ns = ns+1;
    fid = fopen(stimname,'w');
    fprintf(fid,'st=2grating\nsf=%.1f\ntf=%.2f\n',sfone,tf);
    fprintf(fid,'f2=4\n');
    fprintf(fid,'t2=%.2f\n',tfs(j));
    fprintf(fid,'c2=1.0\n');
    fprintf(fid,'nph=0\n');
    fclose(fid);
end
for j = 1:length(jxs)
    stimname = [basedir '/stim' int2str(ns)];
    ns = ns+1;
    fid = fopen(stimname,'w');
    fprintf(fid,'st=rds\ndd=100\nsl=1\n');
    fprintf(fid,'jv=%.2f\n',speed);
    fprintf(fid,'jx=%.2f\n',jxs(j));
    fclose(fid);
end

stimname = [basedir '/stim' int2str(ns)];
ns = ns+1;
fid = fopen(stimname,'w');
fprintf(fid,'st=none\n');
fclose(fid);

nr = round(80./ns);
stimorder = repmat([1:ns]-1,1,nr);
stimorder = stimorder(randperm(length(stimorder)));
stimname = [basedir '/stimorder'];
fid = fopen(stimname,'w');
fprintf(fid,'%s\n',sprintf('%d ',stimorder));
fclose(fid);
fprintf('%d stim * %d repeats\n',ns,nr);