/*
  zip_set_archive_prefix.c -- set archive prefix
  Copyright (C) 2025 Dieter Baron and Thomas Klausner

  This file is part of libzip, a library to manipulate ZIP archives.
  The authors can be contacted at <info@libzip.org>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. The names of the authors may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "zipint.h"
#include <stdlib.h>


ZIP_EXTERN int
zip_set_archive_prefix(zip_t *za, const zip_uint8_t *data, zip_uint64_t length) {
    zip_prefix_t *prefix;

    if (ZIP_IS_RDONLY(za)) {
        zip_error_set(&za->error, ZIP_ER_RDONLY, 0);
        return -1;
    }

    if (data == NULL && length == 0) {
        if (za->prefix_changes != NULL) {
            free(za->prefix_changes);
        }
        za->prefix_changes = NULL;
        za->prefix_changed = 0;
        return 0;
    }

    if ((prefix = (zip_prefix_t *)malloc(sizeof(*prefix))) == NULL) {
        zip_error_set(&za->error, ZIP_ER_MEMORY, 0);
        return -1;
    }

    if (za->prefix_orig > 0 && (za->src->supports & ZIP_SOURCE_SEEK_WRITE) != ZIP_SOURCE_SEEK_WRITE) {
        zip_error_set(&za->error, ZIP_ER_OPNOTSUPP, 0);
        return -1;
    }

    prefix->data = data;
    prefix->length = length;

    za->prefix_changes = prefix;
    za->prefix_changed = 1;

    return 0;
}
