"Inform me of lines that are too long
match ErrorMsg '\%>110v.\+'

"GF command sourcefiles
let &path="src,flat/src,"

nnoremap <F1> :Make -l<cr>
nnoremap <F2> :Make -l checkall<cr>
nnoremap <F3> :Start ./deadgaem<cr>
nnoremap <F4> :Start! ddd deadgaem<cr>

nnoremap <F9> :Make clean<cr>
nnoremap <F10> :Make cleanall<cr>
nnoremap <F12> :Start! tiled<cr>
