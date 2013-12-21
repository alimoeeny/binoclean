function varargout = AllSaccadeExpt(varargin)
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
sacsize=3;
S.types = {'imx'};
S.or = 135;
stimvals{3} = [0];
stimvals{2} = [0];
stimvals{1} = [403004 403004 404004 404004 433008 434005 435002 503002 505006 511007 562001 572001 552001 455001];
distvals = -0.2:0.05:0.2;
ntrials = 1; %number of times to repeat set
basedir = '/local/expts/Saccade';
jumptime = 70;
fixedseed = 1001;

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
    elseif strncmpi(varargin{j},'size',3)
        j = j+1;
        sacsize=varargin{j};
    elseif strncmpi(varargin{j},'ori',5)
        j = j+1;
        S.or = varargin{j};
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
S.str = sprintf('MeanSaccade=%s\n',sprintf('%.3f ',em));
trueem = em .* sacsize(1)./max(abs(em));
seedoffset = round(rand(1,1) .*10000);
distw = length(distvals);
S.stimno = 0;
S.sl = 0;
S.Fs = sacsize;
stimvals{2} = [135 -45];
stimvals{3} = {'mimic' 'grey' 'image'};
nstim = [length(stimvals{1}) length(stimvals{2}) length(stimvals{3}) ntrials/rpts];
ssign = 2.*(round(rand(ntrials.*rpts,1)) -0.5);
sgn = 1;
for t = 1:nstim(4)
for m = 1:nstim(3)
for k = 1:nstim(2)
    for j = 1:nstim(1)
        S.Fs = sacsize;
        trueem = em .* S.Fs./max(abs(em));
        S.Fa = stimvals{2}(k);
        
%         if bitand(j,1)
%             S.St3 = 'none';
%         else
%             S.St3 = 'bar';
%         end  
        S.St3 = 'none';
        oneval = '';
        if strcmp(stimvals{3}{m},'mimic')
            S.sactype = 1;
            if ismember(S.Fa,[0 180])
                S.types = {'imx'};
                oneval = 'imx';
            elseif ismember(S.Fa,[90 -90 270 -270])
                S.types = {'imy'};
                oneval = 'imy';
            else
                oneval = 'imx';
                S.types = {'imx' 'imy'};
            end
            S.Ff = 0;
            S.Bs = 'image';
            S.str = sprintf('MeanSaccade=%s\n',sprintf('%.3f ',em));
        else            
            S.types = {};
            S.Ff = jumptime;
            S.str = '';
            if strcmp(stimvals{3}{m},'grey')
                S.sactype = 2;
                S.Bs= 'none';
                S.St3 = 'none';
            else
                S.sactype = 3;
                S.Bs = 'image';
            end
        end
        
        if ismember(S.Fa,[180 -90])
            sgn = -1;
        else
            sgn = 1;
        end
        imx = zeros(1,nf);
        imy = zeros(1,nf);
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
        imy = imx .* sin(S.Fa * pi/180);
        imx = imx .* cos(S.Fa * pi/180);
        nsig = floor((stimvals{1}(j) .* nf));
        if strcmp(oneval,'imy')
            S.vals{1} = imy;
        else
            S.vals{1} = imx;
        end
        S.vals{2} = imy;
        S.imi = stimvals{1}(j);
        if strcmp(stimvals{3}{m},'mimic') && m < 7
            S.se = fixedseed;
            S.imi = stimvals{1}(1);
        else
            S.se = 1000 + round(rand(1,1).*2000) .*400; %random seed for trial
        end
        S.signal = stimvals{1}(j);
        sname = [basedir '/stim' num2str(S.stimno)];
        WriteStim(S, sname);
        
        AllS(S.stimno+1) = S;
        S.stimno = S.stimno+1;
    end
end
end
end

f = fields(AllS);
for j = 1:length(AllS)
    fprintf('%d:',j-1)
    for k = 1:length(f)
        if isnumeric(AllS(j).(f{k}))
            fprintf(' %s=%.2f',f{k},AllS(j).(f{k}));
        end
    end
    fprintf('\n');
end

stimorder = repmat([0:S.stimno-1],rpts);
stimorder = stimorder(randperm(length(stimorder)));
if nargout > 0
    varargout{1} = AllS;
end
if nargout > 1
    varargout{2} = stimorder;
end

fid = fopen([basedir '/stimorder'],'w');
fprintf(fid,'%d ',stimorder); 
fprintf(fid,'\n');
fclose(fid);

function WriteStim(S, sname)
stimno= 1;
j = 1;


fid = fopen(sname,'w');
fprintf(fid,'%s',S.str);
%fprintf(fid,'xo=0.34\n');
fprintf(fid,'or=%.1f\n',S.or);
fprintf(fid,'Fa=%.1f\n',S.Fa);
fprintf(fid,'Fs=%.1f\n',S.Fs);
fprintf(fid,'Ff=%.1f\n',S.Ff);
fprintf(fid,'se=%d\n',S.se);
fprintf(fid,'imi=%d\n',S.imi);
fprintf(fid,'St3=%s\n',S.St3);
fprintf(fid,'Bs=%s\n',S.Bs);
fprintf(fid,'exvals%.2f %.2f %.2f\n',S.imi,S.Fa,S.sactype);
types = {'imx' 'imy'};
for k = 1:length(S.types)
    fprintf(fid,'%s:',S.types{k});
    for j = 1:length(S.vals{k})
        fprintf(fid,' %.2f',S.vals{k}(j));
    end
    fprintf(fid,'\n');
end
fclose(fid);


