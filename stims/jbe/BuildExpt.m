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
nf = 400;
j = 1;
S.types = {'imx'};
S.or = 0;
stimvals{3} = [0];
stimvals{2} = [0];
stimvals{1} = [403004 419002 433008 434005 435002 503002 505006 511007 562001 572001];
distvals = -0.2:0.05:0.2;
ntrials = 2;
rpts = 1; %5 number of times to show each exact sequence
flipsaccade = 0;

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
    elseif strncmpi(varargin{j},'flip',5)
        flipsaccade = 1;
    elseif strncmpi(varargin{j},'yshift',5)
        S.types = {'imy'};
        S.or = 90;
    elseif strncmpi(varargin{j},'signals',5)
        j = j+1;
        stimvals{1} = varargin{j};
    end
    j = j+1;
end


load('Emavg.mat');
eml = length(em);
em = (em - em(1));
em = em .* 3./max(abs(em));
trueem = em;
seedoffset = round(rand(1,1) .*10000);
distw = length(distvals);
S.stimno = 0;
S.sl = 0;
stimvals{2} = [1 2 4];
nstim = [length(stimvals{1}) length(stimvals{2}) length(stimvals{3}) ntrials/rpts];
ssign = 2.*(round(rand(ntrials.*rpts,1)) -0.5);
sgn = 1;
for t = 1:nstim(4)
for m = 1:nstim(3)
for k = 1:nstim(2)
    for j = 1:nstim(1)
        imx = zeros(1,nf);
        imy = zeros(1,nf);
        sgn = sgn.*-1;
        em = trueem .* sgn;
        imx(70:70+eml-1) = em;
        imx(70+eml:140) = em(end);
        imx(140:140+eml-1) = fliplr(em);
        imx(140+eml:210) = em(1);

        tsacc = 140;
        imx(70+tsacc:70+tsacc+eml-1) = em;
        imx(70+eml+tsacc:140+tsacc) = em(end);
        imx(140+tsacc:140+tsacc+eml-1) = fliplr(em);
        imx(140+tsacc+eml:210+tsacc) = em(1);

        tsacc = 280;
        imx(70+tsacc:70+tsacc+eml-1) = em;
        imx(70+eml+tsacc:140+tsacc) = em(end);
        
        imx(140+tsacc:end) = em(end);

        nsig = floor((stimvals{1}(j) .* nf));
        S.vals{1} = imx;
        S.vals{2}(1:100) = -1;
        S.vals{2}(101:200) = 1;
        S.imi = stimvals{1}(j);
        S.se = 1000 + seedoffset + S.stimno .*200;
        S.dx = stimvals{3}(m);
        S.Fa = S.or+90.*sgn;
        S.wi = stimvals{2}(k);
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
fprintf(fid,'xo=0.34\n');
fprintf(fid,'or=%.1f\n',S.or);
fprintf(fid,'Fa=%.1f\n',S.Fa);
fprintf(fid,'se=%d\n',S.se);
fprintf(fid,'wi=%d\n',S.wi);
fprintf(fid,'imi=%d\n',S.imi);
fprintf(fid,'exvals%.2f %.2f %.2f\n',S.imi,S.wi,0);
types = {'imx' 'imy'};
for k = 1:length(S.types)
    fprintf(fid,'%s:',S.types{k});
    for j = 1:length(S.vals{k})
        fprintf(fid,' %.2f',S.vals{k}(j));
    end
    fprintf(fid,'\n');
end
fclose(fid);


