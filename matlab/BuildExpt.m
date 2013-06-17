function BuildExpt(varargin)
stimno= 1;
nf = 200;
j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'stimno',5)
        j = j+1;
        stimno = varargin{j};
    end
    j = j+1;
end

stimvals{3} = [-0.1 0.1];
stimvals{2} = [-0.1 0.1];
stimvals{1} = 0.8;
S.stimno = 0;
S.sl = 0;
S.types = {'dx' 'ce'};
nstim = [length(stimvals{1}) length(stimvals{2}) length(stimvals{3})];
for m = 1:nstim(3)
for k = 1:nstim(2)
    for j = 1:nstim(1)
        nsig = floor((stimvals{1}(j) .* nf));
        S.vals{1} = round(rand(nf+1,1) .*nstim(1))./(nstim(1) .*10);
        S.vals{1}(1:100) = stimvals{2}(k);
        sigframes = 100 + floor(rand(nsig,1) .* 100);
        S.vals{1}(sigframes) = stimvals{3}(m);
        S.vals{2}(1:100) = -1;
        S.vals{2}(101:200) = 1;
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

rpts = 2;
stimorder = repmat([0:S.stimno-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
fid = fopen('/local/manstim/stimorder','w');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function WriteStim(S)
stimno= 1;
j = 1;


sname = ['/local/manstim/stim' num2str(S.stimno)];
fid = fopen(sname,'w');
fprintf(fid,'psyv%d\n',S.signal);
fprintf(fid,'se%d\n',S.se);
fprintf(fid,'exvals%.2f %.2f %.2f\n',S.dx,S.Dc,S.ac);
fprintf(fid,'dx%0.2f\n',S.dx);
fprintf(fid,'ac%0.2f\n',S.ac);
fprintf(fid,'sl=%d\n',S.sl);
types = {'dx' 'ce'};
for k = 1:length(S.types)
    fprintf(fid,'%s:',S.types{k});
    for j = 1:length(S.vals{k})
        fprintf(fid,' %.2f',S.vals{k}(j));
    end
    fprintf(fid,'\n');
end
fclose(fid);


