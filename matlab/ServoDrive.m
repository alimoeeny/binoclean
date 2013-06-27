function ServoDrive(varargin)
figpos = [1400 400 350 350];

j = 1;
while j <= length(varargin)
    if strncmpi(varargin{j},'callback',5)
        j = j+1;
        DATA.callback = varargin{j};
    elseif strncmpi(varargin{j},'position',5)
        j = j+1;
        figpos = varargin{j};
    elseif strncmpi(varargin{j},'ttyname',3)
        j = j+1;
        DATA.ttyname = varargin{j};
    end
    j = j+1;
end
tag = 'Servo Controller';

[F, isnew] = GetFigure(tag);
if isnew
    set(F,'position',figpos);
    DATA.toplevel = F;
    DATA = SetDefaults(DATA);
    DATA = BuildServoWindow(DATA);
    DATA = OpenServoPort(DATA);
    set(DATA.toplevel,'UserData',DATA);
end

function MoveMicroDrive(a,b,fcn)
DATA = GetDataFromFig(a);
it = findobj(DATA.toplevel,'tag','StepSize');

if strcmp(fcn,'moveto')
    newpos = str2num(get(DATA.setdepth,'string'));
    DATA = SetNewPosition(DATA, newpos);
elseif strcmp(fcn,'set')
    newpos = str2num(get(DATA.setdepth,'string'));
    SetHomePosition(DATA, newpos);
else
    str = get(it,'string');
    j= get(it,'value');
    step = sscanf(str(j,:),'%d');
    if  strcmp(fcn,'Up')
        step = -step;
    end
    fprintf('Moving %d\n',step);
    DATA = ChangePosition(DATA, step);
end
set(DATA.toplevel,'UserData',DATA);


function DATA = BuildServoWindow(DATA)
F = DATA.toplevel;
nr = 7;
nc=6;

set(F,'menubar','none');
im = load('/local/Images/icons/DownArrow.mat');
im.x = im.x-min(im.x(:));
im.x = im.x./max(im.x(:));
[ii,ij] = ind2sub(size(im.x),find(im.x < 0.04));

im.x = 1-im.x;
upx = flipud(im.x);
bcolor = get(gcf,'color');
im.cdata = repmat(upx,[1 1 3]);
im.cdata(find(im.cdata > 0.96)) = mean(bcolor);
bp = [0.1 0.7 0.3 0.2];
uicontrol(gcf,'style','pushbutton','string','Up', ...
     'foregroundcolor','w', ...
   'Callback', {@MoveMicroDrive, 'Up'}, 'Tag','upArrow',...
   'backgroundcolor',bcolor,...
        'units', 'norm', 'position',bp,'value',1,'cdata',im.cdata);

bp = [0.05 0.4 0.3 0.15];
h = uicontrol(gcf,'style','popupmenu','string','10uM|20uM|25uM|50uM|100uM|200uM|Set Custom', ...
        'units', 'norm', 'position',bp,'value',1,...
        'Tag','StepSize','fontsize',18,'fontweight','bold','callback',@SetStepSize);

    bp(2) = 0.2;
    bp(1) = bp(1)+bp(3)+0.05;
bp(3) = 0.15;
uicontrol(gcf,'style','pushbutton','string','Set', ...
   'Callback', {@MoveMicroDrive, 'set'}, 'Tag','SetPosition',...
   'fontsize',18,'fontweight','bold',...
        'units', 'norm', 'position',bp,'value',1);
bp(1) = bp(1)+bp(3);
bp(3) = 0.2;
DATA.setdepth = uicontrol(gcf,'style','edit','string','0', ...
   'fontsize',18,'fontweight','bold',...
        'units', 'norm', 'position',bp,'value',1);
bp(1) = bp(1)+bp(3);
bp(3) = 0.2;
uicontrol(gcf,'style','pushbutton','string','Move', ...
   'Callback', {@MoveMicroDrive, 'moveto'}, 'Tag','DownArrow',...
   'fontsize',18,'fontweight','bold',...
        'units', 'norm', 'position',bp,'value',1);
% 
% bp = [0.5 0.2 0.45 0.15];
%     uicontrol(gcf,'style','pushbutton','string','Reopen', ...
%    'Callback', {@MoveMicroDrive, 'reopen'}, ...
%    'fontsize',18,'fontweight','bold',...
%         'units', 'norm', 'position',bp,'value',1);

bp = [0.1 0.21 0.3 0.2];
im.cdata = repmat(im.x,[1 1 3]);
im.cdata(find(im.cdata > 0.96)) = mean(bcolor);
h = uicontrol(gcf,'style','pushbutton','string','Down', ...
   'Callback', {@MoveMicroDrive, 'down'}, 'Tag','DownArrow',...
        'units', 'norm', 'position',bp,'value',1,...
        'foregroundcolor','w', 'backgroundcolor',bcolor,'cdata',im.cdata);
    bp(1) = bp(1)+bp(3);
    bp(1) = bp(1)+bp(3);
    bp(3) = 0.1;


bp = [0.3 0.01 0.4 0.1];
DATA.depthlabel = uicontrol(gcf,'style','text','string','0 uM', ...
        'units', 'norm', 'position',bp,'value',1,'backgroundcolor',bcolor,'foregroundcolor',[0.8 0.3 0.2],...
        'fontsize',24,'fontweight','bold');
mn = uimenu(F,'Label','Close','callback',@CloseServoPort);
mn = uimenu(F,'Label','Reopen','callback',@OpenServoPort);
mn = uimenu(F,'Label','Stop','callback',@StopMotor);
set(gca,'position',[0.4 0.4 0.6 0.6],'xtick',[],'ytick',[]);
set(DATA.toplevel,'UserData',DATA);


function CloseServoPort(a,b)
DATA = GetDataFromFig(a);
if isfield(DATA, 'sport');
    fclose(DATA.sport);
    delete(DATA.sport);
end
close(DATA.toplevel);


function SetStepSize(a,b)

DATA = GetDataFromFig(a);
val = get(a,'value');
s = get(a,'string');
if strncmp(s(val,:),'Set Custom',8)
    step = DATA.customstep;
    defaultanswer{1} = num2str(step);
    str = inputdlg('Step Size','Custom',1,defaultanswer);
    step = str2num(str{1});
    str = sprintf('%d (Custom)',step);
    DATA.customstep = step;
    DATA.step = step;
    s(val+1,1:length(str)) = str;
    set(a,'string',s,'value',val+1);
else
    DATA.step = sscanf(s(val,:),'%d');
end
set(DATA.toplevel,'UserData',DATA);

function DATA = SetDefaults(DATA)
DATA.position = 0;
DATA.step = 0;
DATA.customstep = 0;
DATA.stepscale = 100;
DATA.motorid = 1;
DATA.alldepths = [];
DATA.alltimes = [];
if ~isfield(DATA,'ttyname')
    DATA.ttyname = '/dev/tty.USA49Wfa1212P1.1';
end
if ~isfield(DATA,'callback')
    DATA.callback = [];
end

function SetHomePosition(DATA, pos)

	fprintf(DATA.sport,sprintf('%dHO%.0f\n', DATA.motorid, pos .* DATA.stepscale));
    set(DATA.setdepth,'string',sprintf('%.0f',pos));
    set(DATA.depthlabel,'string',sprintf('%.0f',pos));
    
    
function DATA = ChangePosition(DATA, step)

d = GetCurrentPostion(DATA);
DATA = SetNewPosition(DATA,d+step);

function d = GetCurrentPostion(DATA)

fprintf(DATA.sport,sprintf('%dPOS\n',DATA.motorid));
pause(0.01);
s = fscanf(DATA.sport,'%s');
d = sscanf(s,'%d');
d = d./DATA.stepscale;

function StopMotor(a,b)
DATA = GetDataFromFig(a);
fprintf(DATA.sport,'%dDI\n',DATA.motorid);


function DATA = SetNewPosition(DATA, pos)

newpos = pos .* DATA.stepscale;
pause(0.01);
fprintf(DATA.sport,'%dPOS\n',DATA.motorid);
pause(0.01);
s = ReadLine(DATA.sport);
d = sscanf(s,'%d');
fprintf('Requesting %.0f->%.0f\n',d,newpos);
fprintf(DATA.sport,'%dEN\n',DATA.motorid);
fprintf(DATA.sport,sprintf('%dLA%.0f\n',DATA.motorid,newpos));
pause(0.01);
fprintf(DATA.sport,'%dM\n',DATA.motorid);
pause(0.01);
ts = now;
npost = 0;
edur = abs(newpos-d) ./2000; %estimated duration
newd(1) = d;
j = 2;
while npost < 2
    fprintf(DATA.sport,sprintf('%dPOS\n',DATA.motorid));
    s = ReadLine(DATA.sport);
    ts(j) = now;
    if ~isempty(s)
        newd(j) = sscanf(s,'%d');
        set(DATA.depthlabel,'string',sprintf('%.0f uM',newd(j)./DATA.stepscale));
        poserr = abs(newd(j)-newpos);
        if poserr < 2
            npost = npost+1;
        end
    else
        newd(j) = 0;
    end
    if mytoc(ts(1)) > edur+1
       npost = 2; 
       yn = questdlg(sprintf('Only Moved to %.3f',newd(end)./(DATA.stepscale.*1000)),'Microdrive Error');
       if strcmp(yn,'Yes')
       end
    end
    j = j+1;
end

dur = (ts(end)-ts(1)).*(24 * 60 *60);
fprintf('End pos %s took %.2f (%.2f) rate %.2f\n',s,dur,edur,(newd(end)-newd(1))/dur);

fprintf(DATA.sport,'%dDI\n',DATA.motorid);
pause(0.01);

DATA.alltimes = [DATA.alltimes ts];
DATA.alldepths = [DATA.alldepths newd];
plot(DATA.alltimes, DATA.alldepths);
set(gca,'xtick',[],'ytick',[]);
axis([min(DATA.alltimes) max(DATA.alltimes) min(DATA.alldepths) max(DATA.alldepths)])
if ~isempty(DATA.callback)
    feval(DATA.callback{:}, newd(end)./DATA.stepscale);
end

function s = ReadLine(port)

while get(port,'BytesAvailable') == 0
    pause(0.001);
end
s = fscanf(port,'%s');


function DATA = OpenServoPort(a,b)
 
DATA = GetDataFromFig(a);
x = instrfind('type','serial');
delete(x);
DATA.sport = serial(DATA.ttyname,'BaudRate',9600);
fopen(DATA.sport);
fprintf(DATA.sport,'ANSW1\n');
fprintf(DATA.sport,'SOR0\n');
fprintf(DATA.sport,'NET1\n');
fprintf(DATA.sport,'BAUD%d\n',9600);
fprintf(DATA.sport,'SP%d\n',50);
fprintf(DATA.sport,'LL%d\n',150000);
fprintf(DATA.sport,'APL1\n');
fprintf(DATA.sport,'%dEN\n',DATA.motorid);

