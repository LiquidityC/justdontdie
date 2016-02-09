"Inform me of lines that are too long
match ErrorMsg '\%>110v.\+'

"GF command sourcefiles
let &path="src,"

nnoremap <F1> :Make -l<cr>
nnoremap <F2> :Make -l checkall<cr>
nnoremap <F3> :!./justdontdie<cr>
nnoremap <F4> :Start gdb justdontdie<cr>

nnoremap <F9> :Make clean<cr>
nnoremap <F10> :Make cleanall<cr>
nnoremap <F12> :Start! tiled<cr>

au FileType cpp,h setl makeprg=make\ -l\ -C\ build
