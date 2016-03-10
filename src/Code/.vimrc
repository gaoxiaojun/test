" VIM基本配置==========================================================

"关闭vi的一致性模式 避免以前版本的一些Bug和局限
set nocompatible
"配置backspace键工作方式
set backspace=indent,eol,start

"显示行号
set number
set numberwidth=1
"设置在编辑过程中右下角显示光标的行列信息
set ruler
"当一行文字很长时取消换行
"set nowrap

"在状态栏显示正在输入的命令
set showcmd

"设置历史记录条数
set history=1000

"设置取消备份 禁止临时文件生成
set nobackup
set noswapfile

"突出现实当前行列
set cursorline
"set cursorcolumn

"设置匹配模式 类似当输入一个左括号时会匹配相应的那个右括号
set showmatch

"设置C/C++方式自动对齐
set autoindent
set cindent

"开启语法高亮功能
syntax enable
syntax on

"指定配色方案为256色
"set t_Co=256

"设置搜索时忽略大小写
"set ignorecase

"设置在Vim中可以使用鼠标 防止在Linux终端下无法拷贝
set mouse=a

"设置Tab宽度
set tabstop=4
"设置自动对齐空格数
set shiftwidth=4
"设置按退格键时可以一次删除4个空格
"set softtabstop=4
"设置按退格键时可以一次删除4个空格
"set smarttab

"将Tab键自动转换成空格 真正需要Tab键时使用[Ctrl + V + Tab]
set expandtab
%retab!

"设置编码方式
set encoding=utf-8
"自动判断编码时 依次尝试一下编码
set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr,latin1

"检测文件类型
filetype on
"针对不同的文件采用不同的缩进方式
filetype indent on
"允许插件
filetype plugin on
"启动智能补全
filetype plugin indent on
"代码长度提示
"set colorcolumn=81
set cc=90
"长度提示颜色 vim
highlight CursorLine cterm=NONE ctermfg=white ctermbg=Black
"长度提示颜色 gvim
"highlight ColorColumn guibg=Black

"turn next file
map <C-n> :bn<CR>

"git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle
" vundle配置===============================================================
set nocompatible               " be iMproved
filetype off                   " required!
 
set rtp+=~/.vim/bundle/vundle/
call vundle#rc()
     
" let Vundle manage Vundle
" required! 
Bundle 'gmarik/vundle'
  
" My Bundles here:
"
" original repos on github
Bundle 'tpope/vim-fugitive'
Bundle 'Lokaltog/vim-easymotion'
Bundle 'rstacruz/sparkup', {'rtp': 'vim/'}
Bundle 'tpope/vim-rails.git'
" vim-scripts repos
Bundle 'L9'
Bundle 'FuzzyFinder'
" non github repos
Bundle 'ctrlp.vim'
" ...
  
filetype plugin indent on     " required!
    "
    " Brief help  -- 此处后面都是vundle的使用命令
    " :BundleList          - list configured bundles
    " :BundleInstall(!)    - install(update) bundles
    " :BundleSearch(!) foo - search(or refresh cache first) for foo
    " :BundleClean(!)      - confirm(or auto-approve) removal of unused bundles
    "
    " see :h vundle for more details or wiki for FAQ
    " NOTE: comments after Bundle command are not allowed..
    "=========================================================================


" vim-powerline Download
Bundle 'https://github.com/Lokaltog/vim-powerline.git'

filetype plugin indent on
"
" vim-powerline setting
set laststatus=2
"set t_Co=256
let g:Powerline_symbols='unicode'
set encoding=utf8

" nerd tree
Bundle 'The-NERD-tree'
nmap <leader>nt :NERDTree<cr>:set rnu<cr>
map <F3> :NERDTreeMirror<CR>
map <F3> :NERDTreeToggle<CR>
let NERDTreeShowBookmarks=1
let NERDTreeShowFiles=1
let NERDTreeShowHidden=1
let NERDTreeIgnore=['\.$','\~$']
let NERDTreeShowLineNumbers=1
let NERDTreeWinPos=1

" nerd commenter
Bundle 'The-NERD-Commenter'

let NERDShutUp=1

"支持单行和多行的选择，//格式
map <c-h> ,c<space>

" 垂直对齐线
Bundle 'https://github.com/nathanaelkane/vim-indent-guides.git'

let g:indent_guides_auto_colors = 0
let g:indent_guides_guide_size = 1 
autocmd VimEnter,Colorscheme * :hi IndentGuidesOdd  guibg=red   ctermbg=3
autocmd VimEnter,Colorscheme * :hi IndentGuidesEven guibg=green ctermbg=4
hi IndentGuidesOdd guibg=red ctermbg=3
hi IndentGuidesEven guibg=green ctermbg=4
