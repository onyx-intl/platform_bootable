/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Copyright (C) 2011 Freescale Semiconductor, Inc. */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define MKE2FS_UTILITY		"/system/bin/mke2fs"
#define LOGE printf
#define LOGW printf

int mkfs_ext4(const char *path)
{
    int rc;
    char *argv[10];

    char *ext4_option = "has_journal,huge_file,flex_bg,uninit_bg,dir_nlink,extra_isize";
    argv[0] = MKE2FS_UTILITY;
    argv[1] = "-O";
    argv[2] = ext4_option;
    argv[3] = "-q";
    argv[4] = (char *)path;
    argv[5] = NULL;
    /* system(3) will cause stack corruption in android.
     * use fork execv instead.
     */

    pid_t pid = fork();

    if (pid == 0) {
	execv(MKE2FS_UTILITY, argv);
	LOGE("Can't run %s (%s)\n", MKE2FS_UTILITY, strerror(errno));
	_exit(-1);
    }

    waitpid(pid, &rc, 0);

    if (!WIFEXITED(rc) || WEXITSTATUS(rc) != 0) {
	LOGE("%s: can't handle non block device \"%s\"\n",
	     __func__, path);
	LOGE("Error in %s \n(Status: %d)\n",
	     MKE2FS_UTILITY, WEXITSTATUS(rc));
	LOGE("Cmd: %s(%s, %s, %s, %s)", argv[0], argv[1],
	     argv[2], argv[3], argv[4]);
	return -1;
    }

    LOGW("format_root return :%d", WEXITSTATUS(rc));
    return 0;
}
