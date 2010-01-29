/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
//#define SORTED_VECTOR 

//#define SORTED_ARRAYS

#ifdef SORTED_VECTOR // timer() -> 928.03
extern "C" {
#include "hashtable_core.h"

#include "stack-def.h"
#include "MALLOC.h"

#include "getfunctionslist.h"
#include "existfunction.h"
#include "addinter.h"
  unsigned int hashtable_core_maxFilled = 0;

  extern int C2F(cvname)(int *,char *,int const*, unsigned long int);
}

#include <iterator>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring> // for std::memset

#include "partition.hxx" // for scilab::core::partition_point_n
#include "unrolled_algorithms.hxx" //for scilab::core::eq_n less_n copy_n

#define MAXLENGHTFUNCTIONNAME 32 /* 24 in fact in scilab +1 for '\0' round to nearest multiple of 8*/

using namespace scilab::core;

namespace {
  struct entry 
  {
    entry() {
      std::memset(key, 0, sizeof(key)+sizeof(data)+1); // no alignement issues, init ints to 0 and namefunction to ""
    }
    entry(int d, char const* name): data(d) {
      int const zero(0);
      if(name) {
	strncpy(namefunction, name, MAXLENGHTFUNCTIONNAME);
      } else {
	namefunction[0]= '\0'; // "" name
      }
      C2F(cvname)(key, namefunction, &zero
		  , static_cast<unsigned long>(strlen(namefunction)));
    }
    int key[nsiz];
    int data;
    char namefunction[MAXLENGHTFUNCTIONNAME];
  };

  /*
   * functions entries are stored in a sorted vector
   */

  typedef std::vector<entry> table_t;

  static  table_t table;

  /*
   * small functors used to search in the vector
   * (could be removed with boost:: or C++0x lambdas )
   * 
   */
  struct key_less_than : std::unary_function<entry const&, bool> {

    key_less_than(int const* const k): key(k) {}

    bool operator()(entry const& e) const { return less_n<nsiz>(key, e.key);}

    int const* const key;
  };

  struct not_less_than_key : std::unary_function<entry const&, bool> {

    not_less_than_key(int const* const k): key(k) {}

    bool operator()(entry const& e) const { return !less_n<nsiz>(e.key, key);}

    int const* const key;
  };


  struct equal_data {
    equal_data(int const d) : data(d) {}
    bool operator()(entry const& e) const { return e.data == data ; }
    int const data;
  };
}

/*
 *================================================================================
 */


/*
 * preallocate memory for  DEFAULT_ELEMENTFUNCTIONLIST entries in table
 */

BOOL create_hashtable_scilab_functions(void) {
  BOOL res;
  try {
    table_t tmp(0);
    tmp.reserve( DEFAULT_ELEMENTFUNCTIONLIST );
    table.swap(tmp);
    res= TRUE;
  } catch( std::bad_alloc& e) {
    res= FALSE;
  }
  return res;
}
/*
 * free memory for table
 */
void destroy_hashtable_scilab_functions() {
  table_t tmp(0); // swap trick needed because resize() does *not* release memory
  table.swap(tmp);
}

BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr
				       , SCI_HFUNCTIONS_ACTION action) {
  BOOL res;
  switch(action) {
  case SCI_HFUNCTIONS_FIND: {
    int tmp[nsiz];
    int const* keyToSearch= key;
    if (name) {
      /* faster than a strcmp */
      int job = 0; /* convert name to id */
      C2F(cvname)(tmp, name, &job, static_cast<unsigned long>(std::strlen(name)));
      keyToSearch = tmp;
    }
    // find lower bound : first elt not <
    table_t::const_iterator i= partition_point_n(table.begin(), table.size(), not_less_than_key(keyToSearch));
    if(i != table.end() && eq_n<nsiz>(keyToSearch, i->key)) {
      *scilab_funptr = i->data;
      res= TRUE;
    } else {
      res= FALSE;
    }
    break;
  }
  case SCI_HFUNCTIONS_BACKSEARCH: { // linear search according to data field in entries
    table_t::const_iterator i= std::find_if(table.begin(), table.end()
					    , equal_data(*scilab_funptr));
    if( i != table.end()) {
      copy_n<nsiz>(i->key, key);
      res= TRUE;
    } else {
      res = FALSE;
    }
    break;
  }
  case SCI_HFUNCTIONS_ENTER: {
    res= FALSE;
    if(table.size() < MAXELEMENTFUNCTIONLIST) {
      entry tmp(*scilab_funptr, name);
      // insert before upper bound : first elt such that key<elt
      table.insert(partition_point_n(table.begin(), table.size()
				     , key_less_than(tmp.key)), tmp);
      if(table.size() > hashtable_core_maxFilled) {
	hashtable_core_maxFilled=  table.size();
      }
      res= TRUE;
    }
    break;
  }
  case SCI_HFUNCTIONS_DELETE: {
    // search entry with given scilab_funptr starting from the lower bound
    // according to given key
    table_t::iterator i(std::find_if(partition_point_n(table.begin(), table.size()
						       , not_less_than_key(key))
				     , table.end(), equal_data(*scilab_funptr)));
    if( i != table.end() ) { // entry found -> erase it
      table.erase(i);
      res= TRUE;
    } else { // not found
      res= FALSE;
    }
    break;
  }
  }
  return res;
}

// small functors, could be removed with boost or c++0X lambdas
struct has_namefunction : std::unary_function<entry const&, bool> {
  bool operator()(entry const& e) const { return e.namefunction[0] != '\0' ;} // <=> strlen(e.namefunction) > 0
};

struct copy_name : std::unary_function<entry const&, char**> {
  copy_name(char** n): names(n) { }
  char** operator()(entry const& e) { 
    if(e.namefunction[0] != '\0') {
      *names= strdup(e.namefunction);
      ++names;
    }
    return names; 
  } // strndup is a GNU extension :(

  char** names;
};

char **GetFunctionsList(int *sizeList) {
  *sizeList= std::count_if(table.begin(), table.end(), has_namefunction());
  char **ListFunctions = static_cast<char**>(MALLOC(sizeof(char*)*(*sizeList)));
  if( ListFunctions ) {
    std::for_each(table.begin(), table.end(), copy_name(ListFunctions));
  }
  return ListFunctions;
}
/*--------------------------------------------------------------------------*/  

struct equal_name : std::unary_function<entry const&, bool>{
  equal_name(char const* const n): name(n){}

  bool operator()(entry const& e) const{ return std::strncmp(e.namefunction, name, MAXLENGHTFUNCTIONNAME) == 0 ;}

  char const* const name;
};

BOOL ExistFunction(char *name) {
  return (std::find_if(table.begin(), table.end(), equal_name(name))==table.end()) 
    ? FALSE : TRUE ;
}


#else // current implementation timer() -> 1022.0119
extern "C"{
#ifdef SORTED_ARRAYS
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <search.h>
#include "MALLOC.h"
#include "stack-def.h"
#include "hashtable_core.h"
#include "getfunctionslist.h"
#include "existfunction.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#define lfind _lfind
#endif
/*--------------------------------------------------------------------------*/
#define MAXLENGHTFUNCTIONNAME nlgh+1 /* 24 in scilab */
typedef struct primitive_table 
{
	unsigned int used;
	int keyIdentity[nsiz];
	int primitivePointer;
	char namefunction[MAXLENGHTFUNCTIONNAME];
} PRIMITIVES_TABLE;
/*--------------------------------------------------------------------------*/
static PRIMITIVES_TABLE *primitiveTableByName = NULL;
static PRIMITIVES_TABLE *primitiveTableByPrimitivePointer = NULL;
static PRIMITIVES_TABLE *primitiveTableByKeyIdentity = NULL;
static PRIMITIVES_TABLE *primitiveTableByUsed = NULL;
static unsigned int primitiveTableSize = 0;
static unsigned int primitiveTableFilled = 0;
/*--------------------------------------------------------------------------*/
extern int C2F(cvname)(int *,char *,int *, unsigned long int);
/*--------------------------------------------------------------------------*/
static BOOL realloc_table_scilab_functions(void);
static BOOL doFindFunction(int *key,char *name, int *scilab_funptr);
static BOOL doBackSearchFunction(int *key, int *scilab_funptr);
static BOOL doEnterFunction(int *key,char *name, int *scilab_funptr);
static BOOL doDeleteFunction(int *key, int *scilab_funptr);
static void deleteEntryInTable(PRIMITIVES_TABLE *table, int index,
							   int (*_PtFuncCompare)(const void *, const void *));
/*--------------------------------------------------------------------------*/
static int compareByName(const void *x, const void *y);
static int compareByPrimitivepointer(const void *x, const void *y);
static int compareByKeyIdentity(const void *x, const void *y);
static int compareByUsed(const void *x, const void *y);
/*--------------------------------------------------------------------------*/
BOOL create_hashtable_scilab_functions(void)
{
	if ((primitiveTableByName) &&
	(primitiveTableByPrimitivePointer) &&
	(primitiveTableByKeyIdentity) &&
	(primitiveTableByUsed)) return FALSE;

	primitiveTableSize = DEFAULT_ELEMENTFUNCTIONLIST;
	primitiveTableFilled = 0;

	/* Default allocation */
	primitiveTableByName = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));
	primitiveTableByPrimitivePointer = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));
	primitiveTableByKeyIdentity = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));
	primitiveTableByUsed = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));

	/* initialization to a empty struct */
	if ((primitiveTableByName) &&
		(primitiveTableByPrimitivePointer) &&
		(primitiveTableByKeyIdentity) &&
		(primitiveTableByUsed))
	{
		unsigned int i = 0;

		PRIMITIVES_TABLE emptyPRIMITIVES_TABLE;
		emptyPRIMITIVES_TABLE.primitivePointer = 0;
		emptyPRIMITIVES_TABLE.used = 0;
		memset(emptyPRIMITIVES_TABLE.keyIdentity, 0, sizeof(int) * nsiz);
		strcpy(emptyPRIMITIVES_TABLE.namefunction, "");

		for(i = 0; i < primitiveTableSize; i++)
		{
			memcpy(&primitiveTableByName[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
		}
		/* copy struct to others tables */
		memcpy(primitiveTableByPrimitivePointer, primitiveTableByName, sizeof(PRIMITIVES_TABLE)*primitiveTableSize);
		memcpy(primitiveTableByKeyIdentity, primitiveTableByName, sizeof(PRIMITIVES_TABLE)*primitiveTableSize);
		memcpy(primitiveTableByUsed, primitiveTableByName, sizeof(PRIMITIVES_TABLE)*primitiveTableSize);

		return TRUE;
	}
	else
	{
		destroy_hashtable_scilab_functions();
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void destroy_hashtable_scilab_functions()
{
	if (primitiveTableByName) {FREE(primitiveTableByName); primitiveTableByName = NULL;}
	if (primitiveTableByPrimitivePointer) {FREE(primitiveTableByPrimitivePointer); primitiveTableByPrimitivePointer = NULL;}
	if (primitiveTableByKeyIdentity) {FREE(primitiveTableByKeyIdentity); primitiveTableByKeyIdentity = NULL;}
	if (primitiveTableByUsed) {FREE(primitiveTableByUsed); primitiveTableByUsed = NULL;}
	primitiveTableSize = 0;
	primitiveTableFilled = 0;
}
/*--------------------------------------------------------------------------*/
BOOL realloc_table_scilab_functions(void)
{
	if (primitiveTableFilled >= primitiveTableSize)
	{
		unsigned int newPrimitiveTableSize = primitiveTableFilled * 2;

		if (newPrimitiveTableSize > MAXELEMENTFUNCTIONLIST) 
		{
			newPrimitiveTableSize = MAXELEMENTFUNCTIONLIST;
		}

		if ((primitiveTableSize != newPrimitiveTableSize) &&
			(primitiveTableSize != MAXELEMENTFUNCTIONLIST))
		{
			primitiveTableByName = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByName, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));
			primitiveTableByPrimitivePointer = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByPrimitivePointer, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));
			primitiveTableByKeyIdentity = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByKeyIdentity, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));
			primitiveTableByUsed = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByUsed, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));

			if (primitiveTableByName && 
				primitiveTableByPrimitivePointer && 
				primitiveTableByKeyIdentity && 
				primitiveTableByUsed)
			{
				unsigned int i = 0;

				PRIMITIVES_TABLE emptyPRIMITIVES_TABLE;
				emptyPRIMITIVES_TABLE.primitivePointer = 0;
				emptyPRIMITIVES_TABLE.used = 0;
				memset(emptyPRIMITIVES_TABLE.keyIdentity, 0, sizeof(int) * nsiz);
				strcpy(emptyPRIMITIVES_TABLE.namefunction, "");

				for (i = primitiveTableSize; i < newPrimitiveTableSize; i++)
				{
					memcpy(&primitiveTableByName[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
					memcpy(&primitiveTableByPrimitivePointer[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
					memcpy(&primitiveTableByKeyIdentity[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
					memcpy(&primitiveTableByUsed[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
				}

				primitiveTableSize = newPrimitiveTableSize;
				return TRUE;
			}
			return FALSE;
		}
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr,
									   SCI_HFUNCTIONS_ACTION action)
{
	switch(action)
	{
	case SCI_HFUNCTIONS_FIND:
		return doFindFunction(key, name, scilab_funptr);

	case SCI_HFUNCTIONS_BACKSEARCH:
		return doBackSearchFunction(key, scilab_funptr);

	case SCI_HFUNCTIONS_ENTER:
		return doEnterFunction(key, name, scilab_funptr);

	case SCI_HFUNCTIONS_DELETE:
		return doDeleteFunction(key, scilab_funptr);
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
char **GetFunctionsList(int *sizeList)
{
	char **ListFunctions = NULL;
	unsigned int i = 0;
	unsigned int j = 0;

	*sizeList = 0;

	/* get numbers of named functions */
	for ( i = 0 ; i < primitiveTableFilled; i++ ) 
	{
		if (primitiveTableByName[i].namefunction) 
		{
			if (strlen(primitiveTableByName[i].namefunction) > 0) j++;
		}
	}
	*sizeList = j;

	ListFunctions = (char**)MALLOC(sizeof(char*)*(*sizeList));
	j = 0;
	if (ListFunctions)
	{
		for ( i = 0 ; i < primitiveTableFilled; i++ ) 
		{
			if (primitiveTableByName[i].namefunction)
			{
				if (strlen(primitiveTableByName[i].namefunction) > 0)
				{
					ListFunctions[j] = strdup(primitiveTableByName[i].namefunction);
					j++;
				}
			}
		}
	}
	return ListFunctions;
}
/*--------------------------------------------------------------------------*/  
BOOL ExistFunction(char *name)
{
	PRIMITIVES_TABLE searched;
	PRIMITIVES_TABLE *found = NULL;
	if (name)
	{
		strcpy(searched.namefunction, name);
		found = static_cast<PRIMITIVES_TABLE *>(bsearch(&searched, primitiveTableByName, 
								primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByName));

		if (found)
		{
			return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doFindFunction(int *key,char *name, int *scilab_funptr)
{
	PRIMITIVES_TABLE searched;
	PRIMITIVES_TABLE *found = NULL;

	if (name)
	{
		strcpy(searched.namefunction, name);
		found =  static_cast<PRIMITIVES_TABLE *>(bsearch(&searched, primitiveTableByName, 
								 primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByName));
	}
	else
	{
		memcpy(searched.keyIdentity, key, sizeof(int) * nsiz);
		found =  static_cast<PRIMITIVES_TABLE *>(bsearch(&searched, primitiveTableByKeyIdentity, 
								 primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity));
	}

	if (found)
	{
		*scilab_funptr = found->primitivePointer;
		return TRUE;
	}

	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doBackSearchFunction(int *key, int *scilab_funptr)
{
	PRIMITIVES_TABLE searched;            
	PRIMITIVES_TABLE *found = NULL;

	searched.primitivePointer = *scilab_funptr;

	found =  static_cast<PRIMITIVES_TABLE *>(bsearch(&searched, primitiveTableByPrimitivePointer, 
							 primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByPrimitivepointer));

	if (found)
	{
		memcpy(key, found->keyIdentity, sizeof(int)* nsiz);
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doEnterFunction(int *key,char *name, int *scilab_funptr)
{
	PRIMITIVES_TABLE newPRIMITIVES_TABLE;

	realloc_table_scilab_functions();

	if (primitiveTableFilled == primitiveTableSize) return FALSE;

	/* add function name by name */
	if (name)
	{
		int zero = 0;
		strcpy(newPRIMITIVES_TABLE.namefunction, name);
		C2F(cvname)(newPRIMITIVES_TABLE.keyIdentity, name, &zero,(unsigned long)strlen(name));
		memcpy(key, newPRIMITIVES_TABLE.keyIdentity, sizeof(int) * nsiz);
	}
	else /* add function by key identity eq. to name */
	{
		strcpy(newPRIMITIVES_TABLE.namefunction, "");
		memcpy(newPRIMITIVES_TABLE.keyIdentity, key, sizeof(int) * nsiz);
	}
	newPRIMITIVES_TABLE.primitivePointer = *scilab_funptr;
	newPRIMITIVES_TABLE.used = 1;

	/* copy in others tables */
	memcpy(&primitiveTableByName[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	memcpy(&primitiveTableByPrimitivePointer[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	memcpy(&primitiveTableByKeyIdentity[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	memcpy(&primitiveTableByUsed[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	primitiveTableFilled++;

	/* sort tables with new entry */
	qsort(primitiveTableByName, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByName);
	qsort(primitiveTableByPrimitivePointer, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByPrimitivepointer);
	qsort(primitiveTableByKeyIdentity, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	qsort(primitiveTableByUsed, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByUsed);

	return TRUE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doDeleteFunction(int *key, int *scilab_funptr)
{
	unsigned int idxMax = 0;

	int idxName = -1;
	int idxPrimitivePointer = -1;
	int idxKeyIdendity = -1;
	int idxUsed = -1;

	PRIMITIVES_TABLE *found = NULL;
	PRIMITIVES_TABLE searched;
	searched.primitivePointer = *scilab_funptr;
	searched.used = 1;

	// delete by KeyIdentity
	memcpy(searched.keyIdentity, key,sizeof(int) * nsiz);
	
	/* search in tables and get index position in each table */
	idxMax = primitiveTableFilled;
	found =  static_cast<PRIMITIVES_TABLE *>(lfind(&searched, primitiveTableByPrimitivePointer, 
						       &idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity));
	if (found)
	{
		if (idxPrimitivePointer == -1) idxPrimitivePointer = abs(found - primitiveTableByPrimitivePointer);
		found = NULL;
	}

	idxMax = primitiveTableFilled;
	found =  static_cast<PRIMITIVES_TABLE *>(lfind(&searched, primitiveTableByKeyIdentity, 
						       &idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity));
	if (found)
	{
		if (idxKeyIdendity == -1) idxKeyIdendity = abs(found - primitiveTableByKeyIdentity);
		found = NULL;
	}

	idxMax = primitiveTableFilled;
	found =  static_cast<PRIMITIVES_TABLE *>(lfind(&searched, primitiveTableByName, 
						       &idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity));
	if (found)
	{
		if (idxName == -1) idxName = abs(found - primitiveTableByName);
		found = NULL;
	}

	idxMax = primitiveTableFilled;
	found =  static_cast<PRIMITIVES_TABLE *>(lfind(&searched, primitiveTableByUsed, 
						       &idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity));
	if (found)
	{
		if (idxUsed == -1) idxUsed = abs(found - primitiveTableByUsed);
		found = NULL;
	}

	if ((idxName >= 0) && (idxPrimitivePointer >= 0) && (idxKeyIdendity >= 0) && (idxUsed >= 0))
	{
		/* delete entry in each table */
		deleteEntryInTable(primitiveTableByName, idxName, compareByName);
		deleteEntryInTable(primitiveTableByPrimitivePointer, idxPrimitivePointer, compareByPrimitivepointer);
		deleteEntryInTable(primitiveTableByKeyIdentity, idxKeyIdendity, compareByKeyIdentity);
		deleteEntryInTable(primitiveTableByUsed, idxUsed, compareByUsed);

		primitiveTableFilled--;

		return TRUE;
	}

	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static int compareByName(const void *x, const void *y)
{
	return strcmp(((PRIMITIVES_TABLE*)x)->namefunction, 
		((PRIMITIVES_TABLE*)y)->namefunction);
}
/*--------------------------------------------------------------------------*/  
static int compareByPrimitivepointer(const void *x, const void *y)
{
	return (((PRIMITIVES_TABLE*)x)->primitivePointer - 
		((PRIMITIVES_TABLE*)y)->primitivePointer);
}
/*--------------------------------------------------------------------------*/  
static int compareByKeyIdentity(const void *x, const void *y)
{
	return memcmp(((PRIMITIVES_TABLE*)x)->keyIdentity, 
		((PRIMITIVES_TABLE*)y)->keyIdentity, sizeof(int)*nsiz);
}
/*--------------------------------------------------------------------------*/  
static int compareByUsed(const void *x, const void *y)
{
	return ((PRIMITIVES_TABLE*)x)->used - ((PRIMITIVES_TABLE*)y)->used;
}
/*--------------------------------------------------------------------------*/  
static void deleteEntryInTable(PRIMITIVES_TABLE *table, int index,
							   int (*_PtFuncCompare)(const void *, const void *))
{
	memset(table[index].keyIdentity, 0, sizeof(int)*nsiz);
	strcpy(table[index].namefunction, "");
	table[index].primitivePointer = 0;
	table[index].used = 0;
	qsort(table, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), *_PtFuncCompare);
}
}

#else
/*--------------------------------------------------------------------------*/  

#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "stack-def.h"
#include "hashtable_core.h"
#include "getfunctionslist.h"
#include "existfunction.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}
/*--------------------------------------------------------------------------*/
#define MAXLENGHTFUNCTIONNAME  256 /* 24 in fact in scilab */

typedef struct entry 
{
  int key[nsiz];
  int data;
  char namefunction[MAXLENGHTFUNCTIONNAME];
} ENTRY;

typedef struct 
{ 
  unsigned int   used;
  ENTRY entry;
} _ENTRY;

/*--------------------------------------------------------------------------*/
static _ENTRY   *htable = NULL;
static unsigned hashtableSize = 0;
unsigned int hashtable_core_maxFilled = 0;
static unsigned filled = 0;
/*--------------------------------------------------------------------------*/
extern "C"{
  extern int C2F(cvname)(int *,char *,int *, unsigned long int);
}
/*--------------------------------------------------------------------------*/
static BOOL Equal_id(int *x, int *y);
static BOOL realloc_hashtable_scilab_functions(void);
/*--------------------------------------------------------------------------*/
BOOL create_hashtable_scilab_functions(void)
{
  unsigned int i = 0;

  if (htable == NULL)
    {
      hashtableSize  = DEFAULT_ELEMENTFUNCTIONLIST;
      filled = 0;

      htable = (_ENTRY *) CALLOC((hashtableSize + 1), sizeof(_ENTRY));

      if (htable)	
	{
	  unsigned int i = 0;
	  htable[0].used = 0;
	  strcpy(htable[0].entry.namefunction, "");
	  for (i = 0; i < nsiz; i++)
	    {
	      htable[0].entry.key[i] = 0;
	    }
	  htable[0].entry.data = 0;

	  for(i = 1; i < hashtableSize; i++)
	    {
	      memcpy(&htable[i], &htable[0], sizeof(htable[0]));
	    }
	  return TRUE;
	}
    }
  return FALSE;
}
/*--------------------------------------------------------------------------*/
void destroy_hashtable_scilab_functions()
{
  if (htable)
    {
      FREE(htable);
      htable = NULL;
    }
}
/*--------------------------------------------------------------------------*/
BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr, SCI_HFUNCTIONS_ACTION action)
{
  switch(action)
    {
    case SCI_HFUNCTIONS_FIND:
      {
	/* linear search algorithm */
	unsigned int idx = 0;
	int keyToSearch[nsiz];

	if (name)
	  {
	    /* faster than a strcmp */
	    int job = 0; /* convert name to id */
	    C2F(cvname)(keyToSearch, name, &job, (unsigned long)strlen(name));
	  }
	else
	  {
	    unsigned int i = 0;
	    for(i = 0; i < nsiz; i++)
	      {
		keyToSearch[i] = key[i];
	      }
	  }

	/* linear search algorithm */
	for ( idx = 0 ; idx < filled + 1; idx++ ) 
	  {
	    if ( htable[idx].used)
	      {
		if (Equal_id(keyToSearch, htable[idx].entry.key) == TRUE)
		  {
		    *scilab_funptr = htable[idx].entry.data;
		    return TRUE;
		  }
	      }
	  }
	return FALSE;
      }
      break;
    case SCI_HFUNCTIONS_BACKSEARCH:
      {
	/* linear search algorithm */
	unsigned int idx = 0;
	for ( idx = 0 ; idx < filled + 1; idx++ ) 
	  {
	    if ( (htable[idx].used) && (htable[idx].entry.data == *scilab_funptr) ) 
	      {
		int j = 0;
		for (j = 0; j < nsiz ; j++ ) key[j] = htable[idx].entry.key[j];
		return TRUE;
	      }
	  }
	return FALSE;
      }
      break;
    case SCI_HFUNCTIONS_ENTER:
      {
	unsigned int idx = 0;

	realloc_hashtable_scilab_functions();

	if (filled ==  hashtableSize) return FALSE;

	for (idx = 0; idx < hashtableSize; idx++)
	  {
	    if (htable[idx].used == 0)
	      {
		int zero = 0;
		int j = 0;
		htable[idx].entry.data = *scilab_funptr;
		if (name) strcpy(htable[idx].entry.namefunction, name);
		else strcpy(htable[idx].entry.namefunction, "");

		C2F(cvname)(htable[idx].entry.key, name, &zero,(unsigned long)strlen(name));

		htable[idx].used = 1;
		filled++;
		if(filled > hashtable_core_maxFilled){
		  hashtable_core_maxFilled = filled;
		}
		return TRUE;
	      }
	  }
	return FALSE;
      }
      break;
    case SCI_HFUNCTIONS_DELETE:
      {
	unsigned int idx = 0;
	for (idx = 0; idx < filled + 1; idx++)
	  {
	    if ( (htable[idx].used) &&
		 (htable[idx].entry.data == *scilab_funptr) &&
		 (Equal_id(key, htable[idx].entry.key) == TRUE) )
	      {
		int i = 0;
		htable[idx].used = 0;
		htable[idx].entry.data = 0;
		strcpy(htable[idx].entry.namefunction, "");
		for (i = 0; i < nsiz; i++)
		  {
		    htable[idx].entry.key[i] = 0;
		  }
		filled--;
		return TRUE;
	      }
	  }
	return FALSE;
      }
      break;
    }
  return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL Equal_id(int *x, int *y)
{
  int i = 0;
  for (i = 0; i < nsiz ; i++ )
    {
      if ( x[i] != y[i] ) return FALSE;
    }
  return TRUE;
}
/*--------------------------------------------------------------------------*/  
char **GetFunctionsList(int *sizeList)
{
  char **ListFunctions = NULL;
  unsigned int i = 0;
  int j = 0;

  *sizeList = 0;

  for ( i = 0 ; i < filled + 1; i++ ) if ( htable[i].used) 
    {
      if (htable[i].entry.namefunction) 
	{
	  if (strlen(htable[i].entry.namefunction) > 0) j++;
	}
    }
  *sizeList = j;

  ListFunctions = (char**)MALLOC(sizeof(char*)*(*sizeList));
  j = 0;
  if (ListFunctions)
    {
      for ( i = 0 ; i < filled + 1; i++ ) if ( htable[i].used) 
	{
	  if (htable[i].entry.namefunction)
	    {
	      if (strlen(htable[i].entry.namefunction) > 0)
		{
		  ListFunctions[j] = strdup(htable[i].entry.namefunction);
		  j++;
		}
	    }
	}
    }
  return ListFunctions;
}
/*--------------------------------------------------------------------------*/  
BOOL ExistFunction(char *name)
{
  int i = 0;

  for ( i = 0 ; i < (int)filled + 1; i++ ) 
    {
      if (htable[i].used) 
	{
	  if (strcmp(htable[i].entry.namefunction, name) == 0)
	    {
	      return TRUE;
	    }
	}
    }
  return FALSE;
}
/*--------------------------------------------------------------------------*/  
BOOL realloc_hashtable_scilab_functions(void)
{
  if ( (filled) >= hashtableSize)
    {
      int newhashtableSize = filled * 2;

      if (newhashtableSize > MAXELEMENTFUNCTIONLIST) newhashtableSize = MAXELEMENTFUNCTIONLIST;

      if ( (hashtableSize != newhashtableSize) && (hashtableSize != MAXELEMENTFUNCTIONLIST) )
	{

	  htable = (_ENTRY *) REALLOC(htable, (newhashtableSize + 1) * sizeof(_ENTRY));
	  if (htable)	
	    {
	      _ENTRY emptyEntry;
	      int i = 0;

	      emptyEntry.used = 0;
	      strcpy(emptyEntry.entry.namefunction, "");
	      for (i = 0; i < nsiz; i++)
		{
		  emptyEntry.entry.key[i] = 0;
		}
	      emptyEntry.entry.data = 0;

	      for (i = hashtableSize; i < newhashtableSize; i++)
		{
		  memcpy(&htable[i], &emptyEntry, sizeof(emptyEntry));
		}
	      hashtableSize = newhashtableSize;
	      return TRUE;
	    }
	  return FALSE;
	}
    }
  return TRUE;
}
/*--------------------------------------------------------------------------*/  

#endif

#endif

