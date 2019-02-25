/******************************************************************************
Copyright 2006 Palmsource, Inc (an ACCESS company).

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/

#include "sqlfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "usage: fuse_sqlfs db.sqlite3 /mount/point/\n");
		exit(1);
	}

    const char* db = argv[1];

	// hack: hide that argument from the rest of the program
	argv[1] = argv[0];
	argv++;
	argc--;

#ifdef HAVE_LIBSQLCIPHER
    sqlfs_t *sqlfs = 0;

/* get the password from stdin */
#  define BUF_SIZE 8192
    char password[BUF_SIZE];
    char *p = fgets(password, BUF_SIZE, stdin);
    if (p)
    {
        /* remove trailing newline */
        size_t last = strlen(p) - 1;
        if (p[last] == '\n')
            p[last] = '\0';
        if (!sqlfs_open_password(db, password, &sqlfs)) {
            fprintf(stderr, "Failed to open: %s\n", db);
            return 1;
        }
        sqlfs_init_password(db, password);
        memset(password, 0, BUF_SIZE); // zero out password
    }
    else
	{
        sqlfs_init(db);
	}
#else
	sqlfs_init(db); // just init it
#endif /* HAVE_LIBSQLCIPHER */

    int rc = sqlfs_fuse_main(argc, argv);
    sqlfs_destroy();
    return rc;
}


/* -*- mode: c; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; c-file-style: "bsd"; -*- */
