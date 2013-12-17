function AllS = GaborNoiseExpt(varargin)

basedir='/local/expts/GaborNoise';
fixedseq  = [160 151 232 412 225 479 225 35 96 279 474 308 407 283 269 5 145 328 436 357 368 377 32 390 9 175 334 292 397 41 38 250 88 45 107 277 45 39 470 255 492 360 357 147 473 203 50 21 198 299 6 151 174 345 169 64 267 453 142 264 398 153 8 373 143 464 3 236 69 143 270 169 407 262 94 441 28 269 316 441 378 187 424 247 66 231 58 144 203 36 196 383 468 470 156 324 460 324 92 426 490 163 128 285 367 315 14 148 61 72 367 231 267 145 421 80 12 300 325 245 497 257 192 274 38 417 127 86 236 116 153 249 422 412 29 72 129 445 125 187 155 213 64 389 105 396 427 3 412 142 55 210 81 28 383 431 397 149 432 367 31 334 37 49 259 366 143 493 290 469 120 423 226 32 38 394 54 495 250 371 177 37 412 121 29 30 355 214 350 421 189 267 374 200 383 234 442 138 357 67];

for j = 1:40
    name = [basedir '/stim' num2str(j-1)];
    fid = fopen(name,'w');
    fprintf(fid,'st=image\n');
    fprintf(fid,'imload=preload\n');
    AllS(j).imi = ceil(rand(1,200).*500);
    if j <= 4
        AllS(j).imi = fixedseq;
    end
    fprintf(fid,'imi:%s\n',sprintf(' %d',AllS(j).imi));
    fclose(fid);
end
for j = 41:80
    name = [basedir '/stim' num2str(j-1)];
    fid = fopen(name,'w');
    AllS(j).dd = 50;
    fprintf(fid,'st=checker\n');
    fprintf(fid,'dd=50\n');
    fprintf(fid,'sz=6\n');    
    fprintf(fid,'sl=2\n');
    if j < 45
        fprintf(fid,'se=1050\n');
    end
        
    fclose(fid);
end
name = [basedir '/stimorder'];

    fid = fopen(name,'w');
stimorder = randperm(length(AllS))-1;
for j = 1:length(stimorder)
    fprintf(fid,'%d ',stimorder(j));
end
fprintf(fid,'\n');
fclose(fid);

