SHLIB=	smallregex
SHLIB_MAJOR=	1
SRCS=	libsmallregex.c
INCS=	libsmallregex.h

.if defined(PREFIX)
INCLUDEDIR?=	${PREFIX}/include
.endif

#LDADD+=	-lmd

CFLAGS+=	-g -O0 -I${.CURDIR}

MAN=

.include <bsd.lib.mk>

