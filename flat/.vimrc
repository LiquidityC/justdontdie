"Inform me of lines that are too long
match ErrorMsg '\%>110v.\+'

"GF command sourcefiles
let &path="src,flat/src,"

nnoremap <F2> :make!<cr>
nnoremap <F3> :make! check<cr>
