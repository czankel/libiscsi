/* 
   Copyright (C) 2013 Ronnie Sahlberg <ronniesahlberg@gmail.com>
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>

#include "iscsi.h"
#include "scsi-lowlevel.h"
#include "iscsi-support.h"
#include "iscsi-test-cu.h"


void
test_verify12_vrprotect(void)
{
	int i, ret;
	unsigned char *buf = alloca(block_size);

	logging(LOG_VERBOSE, LOG_BLANK_LINE);
	logging(LOG_VERBOSE, "Test VERIFY12 with non-zero VRPROTECT");

	CHECK_FOR_SBC;

	if (!inq->protect || (rc16 != NULL && !rc16->prot_en)) {
		logging(LOG_VERBOSE, "Device does not support/use protection information. All commands should fail.");
		for (i = 1; i < 8; i++) {
	
			ret = read10(sd, NULL, 0, block_size,
				     block_size, 0, 0, 0, 0, 0, buf,
				     EXPECT_STATUS_GOOD);
			ret = verify12(sd, 0, block_size,
				       block_size, i, 0, 1, buf,
				       EXPECT_INVALID_FIELD_IN_CDB);
			if (ret == -2) {
				logging(LOG_NORMAL, "[SKIPPED] VERIFY12 is not implemented.");
				CU_PASS("[SKIPPED] Target does not support VERIFY12. Skipping test");
				return;
			}
			CU_ASSERT_EQUAL(ret, 0);
		}
		return;
	}

	logging(LOG_NORMAL, "No tests for devices that support protection information yet.");
}
