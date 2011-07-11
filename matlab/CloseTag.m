function CloseTag(tag)
it = findobj('Tag',tag);
if ~isempty(it)
  close(it);
end

