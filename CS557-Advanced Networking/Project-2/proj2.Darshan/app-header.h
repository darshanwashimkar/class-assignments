/* 
 * File:   app-header.h
 * Author: darshan
 *
 * Created on April 24, 2014, 7:55 PM
 */

#ifndef APP_HEADER_H
#define	APP_HEADER_H

#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "manager.h"
#include "sys/stat.h"
#include "vector"
#include "fstream"
#include "algorithm"
#include "sstream"
#include "sys/time.h"
#include "functional"
#include "tgmath.h"
#include "iomanip"


extern "C"
{   
#include <stdio.h>
#include <ccn/ccn.h>
#include <ccn/uri.h>
#include <ccn/keystore.h>
#include <ccn/signing.h>
#include <ccn/charbuf.h>
#include <ccn/reg_mgmt.h>
#include <ccn/ccn_private.h>
#include <ccn/ccnd.h>
}

#define DEBUG


#endif	/* APP_HEADER_H */

