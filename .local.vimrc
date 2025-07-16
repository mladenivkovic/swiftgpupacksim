
" Set headers to filetype C
augroup c_headers
  autocmd!
  autocmd BufRead,BufNewFile *.h setlocal filetype=c.doxygen
augroup END

