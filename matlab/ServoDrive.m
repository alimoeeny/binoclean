function ServoDrive()
tag = 'Servo Controller';

[F, isnew] = GetFigure(tag);
if isnew
    DATA.toplevel = F;
    DATA = SetDefaults(DATA);
    BuildServoWindow(DATA);
end

function MoveMicroDrive(a,b,fcn)
DATA = GetDataFromFig(a);
it = findobj(DATA.toplevel,'tag','StepSize');
step = str2num(get(it,'string'));
if  strcmp(fcn,'Up')
    step = -step;
end
fprintf('Moving %d\n',step);


function BuildServoWindow(DATA)
F = DATA.toplevel;
nr = 7;
nc=6;

im = load('/local/Images/icons/DownArrow.mat');
im.x = im.x-min(im.x(:));
im.x = im.x./max(im.x(:));
im.cdata = repmat(im.x,[1 1 3]);
bp = [0.01 0.99-1/nr .99/nc 1./nr];
uicontrol(gcf,'style','pushbutton','string','Down', ...
   'Callback', {@MoveMicroDrive, 'down'}, 'Tag','DownArrow',...
        'units', 'norm', 'position',bp,'value',1,'cdata',im.cdata);
    bp(1) = bp(1)+bp(3);
uicontrol(gcf,'style','pushbutton','string','Up', ...
   'Callback', {@MoveMicroDrive, 'Up'}, 'Tag','ApArrow',...
        'units', 'norm', 'position',bp,'value',1);
    bp(1) = bp(1)+bp(3);
    bp(3) = 0.1;

bp = [2/nc 0.99-1/nr 0.99./nc 1./nr];
uicontrol(gcf,'style','text','string','RH', ...
        'units', 'norm', 'position',bp,'value',1);
bp = [2/nc 0.99-1/nr 0.99./nc 1./nr];
uicontrol(gcf,'style','popupmenu','string','10uM|20uM|25uM|50uM|100uM|200uM', ...
        'units', 'norm', 'position',bp,'value',1,'fontsize',18,'fontweight','bold');
 bp(1) = bp(1)+bp(3)+0.01;
    uicontrol(gcf,'style','edit','string','100', ...
        'units', 'norm', 'position',bp,'value',1,'tag','StepSize','fontweight','bold','fontsize',18);

set(DATA.toplevel,'UserData',DATA);


function DATA = SetDefaults(DATA)
DATA.position = 0;

