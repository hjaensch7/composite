#!/bin/sh

./cos_loader \
"c0.o, ;llboot.o, ;*fprr.o, ;mm.o, ;print.o, ;boot.o, ;\
\
!mpool.o,a3;!trans.o,a6;!l.o,a1;!te.o,a3;!e.o,a4;!stat.o,a25;!cbuf.o,a5;(!low.o=micro_ppong.o), ;(!high.o=microcs.o),a10;!va.o,a2;!vm.o,a1;!, :\
\
c0.o-llboot.o;\
fprr.o-print.o|[parent_]mm.o|[faulthndlr_]llboot.o;\
mm.o-[parent_]llboot.o|print.o;\
boot.o-print.o|fprr.o|mm.o|llboot.o;\
l.o-fprr.o|mm.o|print.o;\
te.o-cbuf.o|print.o|fprr.o|mm.o|va.o;\
e.o-cbuf.o|fprr.o|print.o|mm.o|l.o|va.o;\
stat.o-cbuf.o|te.o|fprr.o|l.o|print.o|e.o;\
\
cbuf.o-boot.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o;\
\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
\
vm.o-fprr.o|print.o|mm.o|l.o|boot.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o|vm.o;\
trans.o-fprr.o|l.o|cbuf.o||mm.o|va.o|e.o|print.o;\
\
high.o-cbuf.o|fprr.o|low.o|print.o|mm.o|va.o|l.o;\
low.o-fprr.o|print.o|mm.o|va.o|cbuf.o||l.o\
" ./gen_client_stub
