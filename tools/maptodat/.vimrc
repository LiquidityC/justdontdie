"Inform me of lines that are too long
match ErrorMsg '\%>110v.\+'

"GF command sourcefiles
let &path="src,"

nnoremap <F1> :Make -C build --no-print-directory -l<cr>

nnoremap <F9> :Make -C build --no-print-directory clean<cr>
nnoremap <F10> :Make -C build --no-print-directory clean<cr>

set makeprg=make\ -l\ --no-print-directory\ -C\ build
