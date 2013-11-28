function m = parsevarargin(varargin)
m = containers.Map;
if length(varargin{1})/2  ~= floor(length(varargin{1})/2)
    disp('ERROR parsing varargin, I need even number of key value pairs');
    return 
end

for k = 1:2:length(varargin{1})
    m(varargin{1}{k}) = varargin{1}{k+1};
end
