/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 33 "./CParse/parser.y"

/* doh.h uses #pragma GCC poison with GCC to prevent direct calls to certain
 * standard C library functions being introduced, but those cause errors due
 * to checks like `#if defined YYMALLOC || defined malloc` in the bison
 * template code.  We can't easily arrange to include headers after that
 * template code, so instead we disable the problematic poisoning for this
 * file.
 */
#define DOH_NO_POISON_MALLOC_FREE

#include "swig.h"
#include "cparse.h"
#include "preprocessor.h"
#include <ctype.h>

#define YYMALLOC Malloc
#define YYFREE Free

/* -----------------------------------------------------------------------------
 *                               Externals
 * ----------------------------------------------------------------------------- */

int  yyparse(void);

/* NEW Variables */

static void    *top = 0;      /* Top of the generated parse tree */
static int      unnamed = 0;  /* Unnamed datatype counter */
static Hash    *classes = 0;        /* Hash table of classes */
static Hash    *classes_typedefs = 0; /* Hash table of typedef classes: typedef struct X {...} Y; */
static Symtab  *prev_symtab = 0;
static Node    *current_class = 0;
String  *ModuleName = 0;
static Node    *module_node = 0;
static String  *Classprefix = 0;  
static String  *Namespaceprefix = 0;
static int      inclass = 0;
static Node    *currentOuterClass = 0; /* for nested classes */
static String  *last_cpptype = 0;
static int      inherit_list = 0;
static Parm    *template_parameters = 0;
static int      parsing_template_declaration = 0;
static int      extendmode   = 0;
static int      compact_default_args = 0;
static int      template_reduce = 0;
static int      cparse_externc = 0;
int		ignore_nested_classes = 0;
int		kwargs_supported = 0;

/* -----------------------------------------------------------------------------
 *                            Doxygen Comment Globals
 * ----------------------------------------------------------------------------- */
static String *currentDeclComment = NULL; /* Comment of C/C++ declaration. */

/* -----------------------------------------------------------------------------
 *                            Assist Functions
 * ----------------------------------------------------------------------------- */


 
/* Called by the parser (yyparse) when an error is found.*/
static void yyerror (const char *e) {
  (void)e;
}

static Node *new_node(const_String_or_char_ptr tag) {
  Node *n = Swig_cparse_new_node(tag);
  return n;
}

/* Copies a node.  Does not copy tree links or symbol table data (except for
   sym:name) */

static Node *copy_node(Node *n) {
  Node *nn;
  Iterator k;
  nn = NewHash();
  Setfile(nn,Getfile(n));
  Setline(nn,Getline(n));
  for (k = First(n); k.key; k = Next(k)) {
    String *ci;
    String *key = k.key;
    char *ckey = Char(key);
    if ((strcmp(ckey,"nextSibling") == 0) ||
	(strcmp(ckey,"previousSibling") == 0) ||
	(strcmp(ckey,"parentNode") == 0) ||
	(strcmp(ckey,"lastChild") == 0)) {
      continue;
    }
    if (Strncmp(key,"csym:",5) == 0) continue;
    /* We do copy sym:name.  For templates */
    if ((strcmp(ckey,"sym:name") == 0) || 
	(strcmp(ckey,"sym:weak") == 0) ||
	(strcmp(ckey,"sym:typename") == 0)) {
      String *ci = Copy(k.item);
      Setattr(nn,key, ci);
      Delete(ci);
      continue;
    }
    if (strcmp(ckey,"sym:symtab") == 0) {
      Setattr(nn,"sym:needs_symtab", "1");
    }
    /* We don't copy any other symbol table attributes */
    if (strncmp(ckey,"sym:",4) == 0) {
      continue;
    }
    /* If children.  We copy them recursively using this function */
    if (strcmp(ckey,"firstChild") == 0) {
      /* Copy children */
      Node *cn = k.item;
      while (cn) {
	Node *copy = copy_node(cn);
	appendChild(nn,copy);
	Delete(copy);
	cn = nextSibling(cn);
      }
      continue;
    }
    /* We don't copy the symbol table.  But we drop an attribute 
       requires_symtab so that functions know it needs to be built */

    if (strcmp(ckey,"symtab") == 0) {
      /* Node defined a symbol table. */
      Setattr(nn,"requires_symtab","1");
      continue;
    }
    /* Can't copy nodes */
    if (strcmp(ckey,"node") == 0) {
      continue;
    }
    if ((strcmp(ckey,"parms") == 0) || (strcmp(ckey,"pattern") == 0) || (strcmp(ckey,"throws") == 0)
	|| (strcmp(ckey,"kwargs") == 0)) {
      ParmList *pl = CopyParmList(k.item);
      Setattr(nn,key,pl);
      Delete(pl);
      continue;
    }
    if (strcmp(ckey,"nested:outer") == 0) { /* don't copy outer classes links, they will be updated later */
      Setattr(nn, key, k.item);
      continue;
    }
    /* defaultargs will be patched back in later in update_defaultargs() */
    if (strcmp(ckey,"defaultargs") == 0) {
      Setattr(nn, "needs_defaultargs", "1");
      continue;
    }
    /* same for abstracts, which contains pointers to the source node children, and so will need to be patch too */
    if (strcmp(ckey,"abstracts") == 0) {
      SetFlag(nn, "needs_abstracts");
      continue;
    }
    /* Looks okay.  Just copy the data using Copy */
    ci = Copy(k.item);
    Setattr(nn, key, ci);
    Delete(ci);
  }
  return nn;
}

static void set_comment(Node *n, String *comment) {
  String *name;
  Parm *p;
  if (!n || !comment)
    return;

  if (Getattr(n, "doxygen"))
    Append(Getattr(n, "doxygen"), comment);
  else {
    Setattr(n, "doxygen", comment);
    /* This is the first comment, populate it with @params, if any */
    p = Getattr(n, "parms");
    while (p) {
      if (Getattr(p, "doxygen"))
	Printv(comment, "\n@param ", Getattr(p, "name"), Getattr(p, "doxygen"), NIL);
      p=nextSibling(p);
    }
  }
  
  /* Append same comment to every generated overload */
  name = Getattr(n, "name");
  if (!name)
    return;
  n = nextSibling(n);
  while (n && Getattr(n, "name") && Strcmp(Getattr(n, "name"), name) == 0) {
    Setattr(n, "doxygen", comment);
    n = nextSibling(n);
  }
}

/* -----------------------------------------------------------------------------
 *                              Variables
 * ----------------------------------------------------------------------------- */

static int cplus_mode  = 0;

/* C++ modes */

#define  CPLUS_PUBLIC    1
#define  CPLUS_PRIVATE   2
#define  CPLUS_PROTECTED 3

/* storage classes */

#define SWIG_STORAGE_CLASS_EXTERNC	0x0001
#define SWIG_STORAGE_CLASS_EXTERNCPP	0x0002
#define SWIG_STORAGE_CLASS_EXTERN	0x0004
#define SWIG_STORAGE_CLASS_STATIC	0x0008
#define SWIG_STORAGE_CLASS_TYPEDEF	0x0010
#define SWIG_STORAGE_CLASS_VIRTUAL	0x0020
#define SWIG_STORAGE_CLASS_FRIEND	0x0040
#define SWIG_STORAGE_CLASS_EXPLICIT	0x0080
#define SWIG_STORAGE_CLASS_CONSTEXPR	0x0100
#define SWIG_STORAGE_CLASS_THREAD_LOCAL	0x0200

/* Test if multiple bits are set in x. */
static int multiple_bits_set(unsigned x) { return (x & (x - 1)) != 0; }

static const char* storage_class_string(int c) {
  switch (c) {
    case SWIG_STORAGE_CLASS_EXTERNC:
      return "extern \"C\"";
    case SWIG_STORAGE_CLASS_EXTERNCPP:
      return "extern \"C++\"";
    case SWIG_STORAGE_CLASS_EXTERN:
      return "extern";
    case SWIG_STORAGE_CLASS_STATIC:
      return "static";
    case SWIG_STORAGE_CLASS_TYPEDEF:
      return "typedef";
    case SWIG_STORAGE_CLASS_VIRTUAL:
      return "virtual";
    case SWIG_STORAGE_CLASS_FRIEND:
      return "friend";
    case SWIG_STORAGE_CLASS_EXPLICIT:
      return "explicit";
    case SWIG_STORAGE_CLASS_CONSTEXPR:
      return "constexpr";
    case SWIG_STORAGE_CLASS_THREAD_LOCAL:
      return "thread_local";
  }
  assert(0);
  return "<unknown>";
}

/* include types */
static int   import_mode = 0;

void SWIG_cparse_set_compact_default_args(int defargs) {
  compact_default_args = defargs;
}

int SWIG_cparse_template_reduce(int treduce) {
  template_reduce = treduce;
  return treduce;  
}

/* -----------------------------------------------------------------------------
 *                           Assist functions
 * ----------------------------------------------------------------------------- */

static int promote_type(int t) {
  if (t <= T_UCHAR || t == T_CHAR || t == T_WCHAR) return T_INT;
  return t;
}

/* Perform type-promotion for binary operators */
static int promote(int t1, int t2) {
  t1 = promote_type(t1);
  t2 = promote_type(t2);
  return t1 > t2 ? t1 : t2;
}

static String *yyrename = 0;

/* Forward renaming operator */

static String *resolve_create_node_scope(String *cname, int is_class_definition, int *errored);


Hash *Swig_cparse_features(void) {
  static Hash   *features_hash = 0;
  if (!features_hash) features_hash = NewHash();
  return features_hash;
}

/* -----------------------------------------------------------------------------
 * feature_identifier_fix()
 *
 * If a template, return template with all template parameters fully resolved.
 *
 * This is a copy and modification of typemap_identifier_fix.
 * ----------------------------------------------------------------------------- */

static String *feature_identifier_fix(String *s) {
  String *tp = SwigType_istemplate_templateprefix(s);
  if (tp) {
    String *ts, *ta, *tq;
    ts = SwigType_templatesuffix(s);
    ta = SwigType_templateargs(s);
    tq = Swig_symbol_type_qualify(ta,0);
    Append(tp,tq);
    Append(tp,ts);
    Delete(ts);
    Delete(ta);
    Delete(tq);
    return tp;
  } else {
    return NewString(s);
  }
}

static void set_access_mode(Node *n) {
  if (cplus_mode == CPLUS_PUBLIC)
    Setattr(n, "access", "public");
  else if (cplus_mode == CPLUS_PROTECTED)
    Setattr(n, "access", "protected");
  else
    Setattr(n, "access", "private");
}

static void restore_access_mode(Node *n) {
  String *mode = Getattr(n, "access");
  if (Strcmp(mode, "private") == 0)
    cplus_mode = CPLUS_PRIVATE;
  else if (Strcmp(mode, "protected") == 0)
    cplus_mode = CPLUS_PROTECTED;
  else
    cplus_mode = CPLUS_PUBLIC;
}

/* Generate the symbol table name for an object */
/* This is a bit of a mess. Need to clean up */
static String *add_oldname = 0;



static String *make_name(Node *n, String *name,SwigType *decl) {
  String *made_name = 0;
  int destructor = name && (*(Char(name)) == '~');

  if (yyrename) {
    String *s = NewString(yyrename);
    Delete(yyrename);
    yyrename = 0;
    if (destructor  && (*(Char(s)) != '~')) {
      Insert(s,0,"~");
    }
    return s;
  }

  if (!name) return 0;

  if (parsing_template_declaration)
    SetFlag(n, "parsing_template_declaration");
  made_name = Swig_name_make(n, Namespaceprefix, name, decl, add_oldname);
  Delattr(n, "parsing_template_declaration");

  return made_name;
}

/* Generate an unnamed identifier */
static String *make_unnamed(void) {
  unnamed++;
  return NewStringf("$unnamed%d$",unnamed);
}

static int is_operator(String *name) {
  return Strncmp(name,"operator ", 9) == 0;
}

/* Add declaration list to symbol table */
static int  add_only_one = 0;

static void add_symbols(Node *n) {
  String *decl;
  String *wrn = 0;

  if (inclass && n) {
    cparse_normalize_void(n);
  }
  while (n) {
    String *symname = 0;
    String *old_prefix = 0;
    Symtab *old_scope = 0;
    int isfriend = inclass && Strstr(Getattr(n, "storage"), "friend") != NULL;
    int iscdecl = Cmp(nodeType(n),"cdecl") == 0;
    int only_csymbol = 0;
    
    if (inclass) {
      String *name = Getattr(n, "name");
      if (isfriend) {
	/* Friends methods in a class are declared in the namespace enclosing the class (outer most class if a nested class) */
	String *prefix = name ? Swig_scopename_prefix(name) : 0;
	Node *outer = currentOuterClass;
	Symtab *namespace_symtab;
	old_prefix = Namespaceprefix;
	old_scope = Swig_symbol_current();

	assert(outer);
	while (Getattr(outer, "nested:outer")) {
	  outer = Getattr(outer, "nested:outer");
	}
	namespace_symtab = Getattr(outer, "sym:symtab");
	if (!namespace_symtab)
	  namespace_symtab = Getattr(outer, "unofficial:symtab");
	Swig_symbol_setscope(namespace_symtab);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);

	if (!prefix) {
	  /* To check - this should probably apply to operators too */
	  if (name && !is_operator(name) && Namespaceprefix) {
	    String *friendusing = NewStringf("using namespace %s;", Namespaceprefix);
	    Setattr(n, "friendusing", friendusing);
	    Delete(friendusing);
	  }
	} else {
	  /* Qualified friend declarations should not be possible as they are ignored in the parse tree */
	  assert(0);
	}
      } else if (Equal(nodeType(n), "using")) {
	String *uname = Getattr(n, "uname");
	Node *cls = currentOuterClass;
	String *nprefix = 0;
	String *nlast = 0;
	Swig_scopename_split(uname, &nprefix, &nlast);
	if (Swig_item_in_list(Getattr(cls, "baselist"), nprefix) || Swig_item_in_list(Getattr(cls, "protectedbaselist"), nprefix) || Swig_item_in_list(Getattr(cls, "privatebaselist"), nprefix)) {
	  String *plain_name = SwigType_istemplate(nprefix) ? SwigType_templateprefix(nprefix) : nprefix;
	  if (Equal(nlast, plain_name)) {
	    /* Using declaration looks like it is using a constructor in an immediate base class - change the constructor name for this class.
	     * C++11 requires using declarations for inheriting base constructors to be in the immediate base class.
	     * Note that we don't try and look up the constructor in the base class as the constructor may be an implicit/implied constructor and hence not exist. */
	    Symtab *stab = Swig_symbol_current();
	    String *nname = Getattr(stab, "name");
	    Setattr(n, "name", nname);
	    SetFlag(n, "usingctor");
	  }
	}
      } else {
	/* for member functions, we need to remove the redundant
	   class scope if provided, as in
	   
	   struct Foo {
	   int Foo::method(int a);
	   };
	   
	*/
	String *prefix = name ? Swig_scopename_prefix(name) : 0;
	if (prefix) {
	  if (Classprefix && (Equal(prefix,Classprefix))) {
	    String *base = Swig_scopename_last(name);
	    Setattr(n,"name",base);
	    Delete(base);
	  }
	  Delete(prefix);
	}
      }
    }

    if (!isfriend && (inclass || extendmode)) {
      Setattr(n,"ismember","1");
    }

    if (extendmode) {
      if (!Getattr(n, "template"))
        SetFlag(n,"isextendmember");
    }

    if (!isfriend && inclass) {
      if ((cplus_mode != CPLUS_PUBLIC)) {
	only_csymbol = 1;
	if (cplus_mode == CPLUS_PROTECTED) {
	  Setattr(n,"access", "protected");
	  only_csymbol = !Swig_need_protected(n);
	} else {
	  Setattr(n,"access", "private");
	  /* private are needed only when they are pure virtuals - why? */
	  if ((Cmp(Getattr(n,"storage"),"virtual") == 0) && (Cmp(Getattr(n,"value"),"0") == 0)) {
	    only_csymbol = 0;
	  }
	  if (Cmp(nodeType(n),"destructor") == 0) {
	    /* Needed for "unref" feature */
	    only_csymbol = 0;
	  }
	}
      } else {
	Setattr(n, "access", "public");
      }
    } else if (extendmode && !inclass) {
      Setattr(n, "access", "public");
    }

    if (Getattr(n,"sym:name")) {
      n = nextSibling(n);
      continue;
    }
    decl = Getattr(n,"decl");
    if (!SwigType_isfunction(decl)) {
      String *name = Getattr(n,"name");
      String *makename = Getattr(n,"parser:makename");
      if (iscdecl) {	
	String *storage = Getattr(n, "storage");
	if (Cmp(storage,"typedef") == 0) {
	  Setattr(n,"kind","typedef");
	} else {
	  SwigType *type = Getattr(n,"type");
	  String *value = Getattr(n,"value");
	  Setattr(n,"kind","variable");
	  if (value && Len(value)) {
	    Setattr(n,"hasvalue","1");
	  }
	  if (type) {
	    SwigType *ty;
	    SwigType *tmp = 0;
	    if (decl) {
	      ty = tmp = Copy(type);
	      SwigType_push(ty,decl);
	    } else {
	      ty = type;
	    }
	    if (storage && (Strstr(storage, "constexpr") || (Strstr(storage, "static") && !SwigType_ismutable(ty)))) {
	      SetFlag(n, "hasconsttype");
	    }
	    Delete(tmp);
	  }
	  if (!type) {
	    Printf(stderr,"notype name %s\n", name);
	  }
	}
      }
      Swig_features_get(Swig_cparse_features(), Namespaceprefix, name, 0, n);
      if (makename) {
	symname = make_name(n, makename,0);
        Delattr(n,"parser:makename"); /* temporary information, don't leave it hanging around */
      } else {
        makename = name;
	symname = make_name(n, makename,0);
      }
      
      if (!symname) {
	symname = Copy(Getattr(n,"unnamed"));
      }
      if (symname) {
	if (parsing_template_declaration)
	  SetFlag(n, "parsing_template_declaration");
	wrn = Swig_name_warning(n, Namespaceprefix, symname,0);
	Delattr(n, "parsing_template_declaration");
      }
    } else {
      String *name = Getattr(n,"name");
      SwigType *fdecl = Copy(decl);
      SwigType *fun = SwigType_pop_function(fdecl);
      if (iscdecl) {	
	Setattr(n,"kind","function");
      }
      
      Swig_features_get(Swig_cparse_features(),Namespaceprefix,name,fun,n);

      symname = make_name(n, name,fun);
      if (parsing_template_declaration)
	SetFlag(n, "parsing_template_declaration");
      wrn = Swig_name_warning(n, Namespaceprefix,symname,fun);
      Delattr(n, "parsing_template_declaration");
      
      Delete(fdecl);
      Delete(fun);
      
    }
    if (!symname) {
      n = nextSibling(n);
      continue;
    }

    if (GetFlag(n, "valueignored")) {
      SWIG_WARN_NODE_BEGIN(n);
      Swig_warning(WARN_PARSE_ASSIGNED_VALUE, Getfile(n), Getline(n), "Value assigned to %s not used due to limited parsing implementation.\n", SwigType_namestr(Getattr(n, "name")));
      SWIG_WARN_NODE_END(n);
    }

    if (cparse_cplusplus) {
      String *value = Getattr(n, "value");
      if (value && Strcmp(value, "delete") == 0) {
	/* C++11 deleted definition / deleted function */
        SetFlag(n,"deleted");
        SetFlag(n,"feature:ignore");
      }
      if (SwigType_isrvalue_reference(Getattr(n, "refqualifier"))) {
	/* Ignore rvalue ref-qualifiers by default
	 * Use Getattr instead of GetFlag to handle explicit ignore and explicit not ignore */
	if (!(Getattr(n, "feature:ignore") || Strncmp(symname, "$ignore", 7) == 0)) {
	  SWIG_WARN_NODE_BEGIN(n);
	  Swig_warning(WARN_TYPE_RVALUE_REF_QUALIFIER_IGNORED, Getfile(n), Getline(n),
	      "Method with rvalue ref-qualifier %s ignored.\n", Swig_name_decl(n));
	  SWIG_WARN_NODE_END(n);
	  SetFlag(n, "feature:ignore");
	}
      }
      if (Equal(Getattr(n, "type"), "auto")) {
	/* Ignore functions with an auto return type and no trailing return type
	 * Use Getattr instead of GetFlag to handle explicit ignore and explicit not ignore */
	if (!(Getattr(n, "feature:ignore") || Strncmp(symname, "$ignore", 7) == 0)) {
	  SWIG_WARN_NODE_BEGIN(n);
	  if (SwigType_isfunction(Getattr(n, "decl")))
	    Swig_warning(WARN_CPP14_AUTO, Getfile(n), Getline(n), "Unable to deduce auto return type for '%s' (ignored).\n", Swig_name_decl(n));
	  else
	    Swig_warning(WARN_CPP11_AUTO, Getfile(n), Getline(n), "Unable to deduce auto type for variable '%s' (ignored).\n", Swig_name_decl(n));
	  SWIG_WARN_NODE_END(n);
	  SetFlag(n, "feature:ignore");
	}
      }
    }
    if (only_csymbol || GetFlag(n, "feature:ignore") || Strncmp(symname, "$ignore", 7) == 0) {
      /* Only add to C symbol table and continue */
      Swig_symbol_add(0, n);
      if (!only_csymbol && !GetFlag(n, "feature:ignore")) {
	/* Print the warning attached to $ignore name, if any */
        char *c = Char(symname) + 7;
	if (strlen(c)) {
	  SWIG_WARN_NODE_BEGIN(n);
	  Swig_warning(0,Getfile(n), Getline(n), "%s\n",c+1);
	  SWIG_WARN_NODE_END(n);
	}
	/* If the symbol was ignored via "rename" and is visible, set also feature:ignore*/
	SetFlag(n, "feature:ignore");
      }
      if (!GetFlag(n, "feature:ignore") && Strcmp(symname,"$ignore") == 0) {
	/* Add feature:ignore if the symbol was explicitly ignored, regardless of visibility */
	SetFlag(n, "feature:ignore");
      }
    } else {
      Node *c;
      if ((wrn) && (Len(wrn))) {
	String *metaname = symname;
	if (!Getmeta(metaname,"already_warned")) {
	  SWIG_WARN_NODE_BEGIN(n);
	  Swig_warning(0,Getfile(n),Getline(n), "%s\n", wrn);
	  SWIG_WARN_NODE_END(n);
	  Setmeta(metaname,"already_warned","1");
	}
      }
      c = Swig_symbol_add(symname,n);

      if (c != n) {
	/* symbol conflict attempting to add in the new symbol */
	if (Getattr(n,"sym:weak")) {
	  Setattr(n,"sym:name",symname);
	} else {
	  Swig_symbol_conflict_warn(n, c, symname, inclass);
        }
      }
    }
    /* restore the class scope if needed */
    if (isfriend) {
      Swig_symbol_setscope(old_scope);
      if (old_prefix) {
	Delete(Namespaceprefix);
	Namespaceprefix = old_prefix;
      }
    }
    Delete(symname);

    if (add_only_one) return;
    n = nextSibling(n);
  }
}


/* add symbols a parse tree node copy */

static void add_symbols_copy(Node *n) {
  int    emode = 0;
  while (n) {
    if (Equal(nodeType(n), "access")) {
      String *kind = Getattr(n,"kind");
      if (Strcmp(kind,"public") == 0) {
	cplus_mode = CPLUS_PUBLIC;
      } else if (Strcmp(kind,"private") == 0) {
	cplus_mode = CPLUS_PRIVATE;
      } else if (Strcmp(kind,"protected") == 0) {
	cplus_mode = CPLUS_PROTECTED;
      }
      n = nextSibling(n);
      continue;
    }

    add_oldname = Getattr(n,"sym:name");
    if ((add_oldname) || (Getattr(n,"sym:needs_symtab"))) {
      int old_inclass = -1;
      Node *oldCurrentOuterClass = 0;
      if (add_oldname) {
	DohIncref(add_oldname);
	/*  Disable this, it prevents %rename to work with templates */
	/* If already renamed, we used that name  */
	/*
	if (Strcmp(add_oldname, Getattr(n,"name")) != 0) {
	  Delete(yyrename);
	  yyrename = Copy(add_oldname);
	}
	*/
      }
      Delattr(n,"sym:needs_symtab");
      Delattr(n,"sym:name");

      add_only_one = 1;
      add_symbols(n);

      if (Getattr(n,"partialargs")) {
	Swig_symbol_cadd(Getattr(n,"partialargs"),n);
      }
      add_only_one = 0;
      if (Equal(nodeType(n), "class")) {
	/* add_symbols() above sets "sym:symtab", so "unofficial:symtab" is not required */
	old_inclass = inclass;
	oldCurrentOuterClass = currentOuterClass;
	inclass = 1;
	currentOuterClass = n;
	if (Strcmp(Getattr(n,"kind"),"class") == 0) {
	  cplus_mode = CPLUS_PRIVATE;
	} else {
	  cplus_mode = CPLUS_PUBLIC;
	}
      }
      if (Equal(nodeType(n), "extend")) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }

      if (Getattr(n, "requires_symtab")) {
	Swig_symbol_newscope();
	Swig_symbol_setscopename(Getattr(n, "name"));
	Delete(Namespaceprefix);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }

      add_symbols_copy(firstChild(n));

      if (Equal(nodeType(n), "extend")) {
	cplus_mode = emode;
      }
      if (Getattr(n,"requires_symtab")) {
	Setattr(n,"symtab", Swig_symbol_popscope());
	Delattr(n,"requires_symtab");
	Delete(Namespaceprefix);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (add_oldname) {
	Delete(add_oldname);
	add_oldname = 0;
      }
      if (Equal(nodeType(n), "class")) {
	inclass = old_inclass;
	currentOuterClass = oldCurrentOuterClass;
      }
    } else {
      if (Equal(nodeType(n), "extend")) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (Equal(nodeType(n), "extend")) {
	cplus_mode = emode;
      }
    }
    n = nextSibling(n);
  }
}

/* Add in the "defaultargs" attribute for functions in instantiated templates.
 * n should be any instantiated template (class or start of linked list of functions). */
static void update_defaultargs(Node *n) {
  if (n) {
    Node *firstdefaultargs = n;
    update_defaultargs(firstChild(n));
    n = nextSibling(n);
    /* recursively loop through nodes of all types, but all we really need are the overloaded functions */
    while (n) {
      update_defaultargs(firstChild(n));
      if (!Getattr(n, "defaultargs")) {
	if (Getattr(n, "needs_defaultargs")) {
	  Setattr(n, "defaultargs", firstdefaultargs);
	  Delattr(n, "needs_defaultargs");
	} else {
	  firstdefaultargs = n;
	}
      } else {
	/* Functions added in with %extend (for specialized template classes) will already have default args patched up */
	assert(Getattr(n, "defaultargs") == firstdefaultargs);
      }
      n = nextSibling(n);
    }
  }
}

/* Check a set of declarations to see if any are pure-abstract */

static List *pure_abstracts(Node *n) {
  List *abstracts = 0;
  while (n) {
    if (Cmp(nodeType(n),"cdecl") == 0) {
      String *decl = Getattr(n,"decl");
      if (SwigType_isfunction(decl)) {
	String *init = Getattr(n,"value");
	if (Cmp(init,"0") == 0) {
	  if (!abstracts) {
	    abstracts = NewList();
	  }
	  Append(abstracts,n);
	  SetFlag(n,"abstract");
	}
      }
    } else if (Cmp(nodeType(n),"destructor") == 0) {
      if (Cmp(Getattr(n,"value"),"0") == 0) {
	if (!abstracts) {
	  abstracts = NewList();
	}
	Append(abstracts,n);
	SetFlag(n,"abstract");
      }
    }
    n = nextSibling(n);
  }
  return abstracts;
}

/* Recompute the "abstracts" attribute for the classes in instantiated templates, similarly to update_defaultargs() above. */
static void update_abstracts(Node *n) {
  for (; n; n = nextSibling(n)) {
    Node* const child = firstChild(n);
    if (!child)
      continue;

    update_abstracts(child);

    if (Getattr(n, "needs_abstracts")) {
      Setattr(n, "abstracts", pure_abstracts(child));
      Delattr(n, "needs_abstracts");
    }
  }
}

/* Make a classname */

static String *make_class_name(String *name) {
  String *nname = 0;
  String *prefix;
  if (Namespaceprefix) {
    nname= NewStringf("%s::%s", Namespaceprefix, name);
  } else {
    nname = NewString(name);
  }
  prefix = SwigType_istemplate_templateprefix(nname);
  if (prefix) {
    String *args, *qargs;
    args   = SwigType_templateargs(nname);
    qargs  = Swig_symbol_type_qualify(args,0);
    Append(prefix,qargs);
    Delete(nname);
    Delete(args);
    Delete(qargs);
    nname = prefix;
  }
  return nname;
}

/* Use typedef name as class name */

static void add_typedef_name(Node *n, Node *declnode, String *oldName, Symtab *cscope, String *scpname) {
  String *class_rename = 0;
  SwigType *decl = Getattr(declnode, "decl");
  if (!decl || !Len(decl)) {
    String *cname;
    String *tdscopename;
    String *class_scope = Swig_symbol_qualifiedscopename(cscope);
    String *name = Getattr(declnode, "name");
    cname = Copy(name);
    Setattr(n, "tdname", cname);
    tdscopename = class_scope ? NewStringf("%s::%s", class_scope, name) : Copy(name);
    class_rename = Getattr(n, "class_rename");
    if (class_rename && (Strcmp(class_rename, oldName) == 0))
      Setattr(n, "class_rename", NewString(name));
    if (!classes_typedefs) classes_typedefs = NewHash();
    if (!Equal(scpname, tdscopename) && !Getattr(classes_typedefs, tdscopename)) {
      Setattr(classes_typedefs, tdscopename, n);
    }
    Setattr(n, "decl", decl);
    Delete(class_scope);
    Delete(cname);
    Delete(tdscopename);
  }
}

/* If the class name is qualified.  We need to create or lookup namespace entries */

static Symtab *set_scope_to_global(void) {
  Symtab *symtab = Swig_symbol_global_scope();
  Swig_symbol_setscope(symtab);
  return symtab;
}
 
/* Remove the block braces, { and }, if the 'noblock' attribute is set.
 * Node *kw can be either a Hash or Parmlist. */
static String *remove_block(Node *kw, const String *inputcode) {
  String *modified_code = 0;
  while (kw) {
   String *name = Getattr(kw,"name");
   if (name && (Cmp(name,"noblock") == 0)) {
     char *cstr = Char(inputcode);
     int len = Len(inputcode);
     if (len && cstr[0] == '{') {
       --len; ++cstr; 
       if (len && cstr[len - 1] == '}') { --len; }
       /* we now remove the extra spaces */
       while (len && isspace((int)cstr[0])) { --len; ++cstr; }
       while (len && isspace((int)cstr[len - 1])) { --len; }
       modified_code = NewStringWithSize(cstr, len);
       break;
     }
   }
   kw = nextSibling(kw);
  }
  return modified_code;
}

/*
#define RESOLVE_DEBUG 1
*/
static Node *nscope = 0;
static Node *nscope_inner = 0;

/* Remove the scope prefix from cname and return the base name without the prefix.
 * The scopes required for the symbol name are resolved and/or created, if required.
 * For example AA::BB::CC as input returns CC and creates the namespace AA then inner 
 * namespace BB in the current scope. */
static String *resolve_create_node_scope(String *cname_in, int is_class_definition, int *errored) {
  Symtab *gscope = 0;
  Node *cname_node = 0;
  String *cname = cname_in;
  String *last = Swig_scopename_last(cname);
  nscope = 0;
  nscope_inner = 0;  
  *errored = 0;

  if (Strncmp(cname, "::", 2) == 0) {
    if (is_class_definition) {
      Swig_error(cparse_file, cparse_line, "Using the unary scope operator :: in class definition '%s' is invalid.\n", SwigType_namestr(cname));
      *errored = 1;
      return last;
    }
    cname = NewString(Char(cname) + 2);
  }
  if (is_class_definition) {
    /* Only lookup symbols which are in scope via a using declaration but not via a using directive.
       For example find y via 'using x::y' but not y via a 'using namespace x'. */
    cname_node = Swig_symbol_clookup_no_inherit(cname, 0);
    if (!cname_node) {
      Node *full_lookup_node = Swig_symbol_clookup(cname, 0);
      if (full_lookup_node) {
       /* This finds a symbol brought into scope via both a using directive and a using declaration. */
	Node *last_node = Swig_symbol_clookup_no_inherit(last, 0);
	if (last_node == full_lookup_node)
	  cname_node = last_node;
      }
    }
  } else {
    /* For %template, the template needs to be in scope via any means. */
    cname_node = Swig_symbol_clookup(cname, 0);
  }
#if RESOLVE_DEBUG
  if (!cname_node)
    Printf(stdout, "symbol does not yet exist (%d): [%s]\n", is_class_definition, cname_in);
  else
    Printf(stdout, "symbol does exist (%d): [%s]\n", is_class_definition, cname_in);
#endif

  if (cname_node) {
    /* The symbol has been defined already or is in another scope.
       If it is a weak symbol, it needs replacing and if it was brought into the current scope,
       the scope needs adjusting appropriately for the new symbol.
       Similarly for defined templates. */
    Symtab *symtab = Getattr(cname_node, "sym:symtab");
    Node *sym_weak = Getattr(cname_node, "sym:weak");
    if ((symtab && sym_weak) || Equal(nodeType(cname_node), "template")) {
      /* Check if the scope is the current scope */
      String *current_scopename = Swig_symbol_qualifiedscopename(0);
      String *found_scopename = Swig_symbol_qualifiedscopename(symtab);
      if (!current_scopename)
	current_scopename = NewString("");
      if (!found_scopename)
	found_scopename = NewString("");

      {
	int fail = 1;
	List *current_scopes = Swig_scopename_tolist(current_scopename);
	List *found_scopes = Swig_scopename_tolist(found_scopename);
        Iterator cit = First(current_scopes);
	Iterator fit = First(found_scopes);
#if RESOLVE_DEBUG
Printf(stdout, "comparing current: [%s] found: [%s]\n", current_scopename, found_scopename);
#endif
	for (; fit.item && cit.item; fit = Next(fit), cit = Next(cit)) {
	  String *current = cit.item;
	  String *found = fit.item;
#if RESOLVE_DEBUG
	  Printf(stdout, "  looping %s %s\n", current, found);
#endif
	  if (Strcmp(current, found) != 0)
	    break;
	}

	if (!cit.item) {
	  String *subscope = NewString("");
	  for (; fit.item; fit = Next(fit)) {
	    if (Len(subscope) > 0)
	      Append(subscope, "::");
	    Append(subscope, fit.item);
	  }
	  if (Len(subscope) > 0)
	    cname = NewStringf("%s::%s", subscope, last);
	  else
	    cname = Copy(last);
#if RESOLVE_DEBUG
	  Printf(stdout, "subscope to create: [%s] cname: [%s]\n", subscope, cname);
#endif
	  fail = 0;
	  Delete(subscope);
	} else {
	  if (is_class_definition) {
	    if (!fit.item) {
	      /* It is valid to define a new class with the same name as one forward declared in a parent scope */
	      fail = 0;
	    } else if (Swig_scopename_check(cname)) {
	      /* Classes defined with scope qualifiers must have a matching forward declaration in matching scope */
	      fail = 1;
	    } else {
	      /* This may let through some invalid cases */
	      fail = 0;
	    }
#if RESOLVE_DEBUG
	    Printf(stdout, "scope for class definition, fail: %d\n", fail);
#endif
	  } else {
#if RESOLVE_DEBUG
	    Printf(stdout, "no matching base scope for template\n");
#endif
	    fail = 1;
	  }
	}

	Delete(found_scopes);
	Delete(current_scopes);

	if (fail) {
	  String *cname_resolved = NewStringf("%s::%s", found_scopename, last);
	  Swig_error(cparse_file, cparse_line, "'%s' resolves to '%s' and was incorrectly instantiated in scope '%s' instead of within scope '%s'.\n",
	    SwigType_namestr(cname_in), SwigType_namestr(cname_resolved), SwigType_namestr(current_scopename), SwigType_namestr(found_scopename));
	  *errored = 1;
	  Delete(cname_resolved);
	}
      }

      Delete(current_scopename);
      Delete(found_scopename);
    }
  } else if (!is_class_definition) {
    /* A template instantiation requires a template to be found in scope */
    Swig_error(cparse_file, cparse_line, "Template '%s' undefined.\n", SwigType_namestr(cname_in));
    *errored = 1;
  }

  if (*errored)
    return last;

  if (Swig_scopename_check(cname) && !*errored) {
    Node   *ns;
    String *prefix = Swig_scopename_prefix(cname);
    if (Len(prefix) == 0) {
      String *base = Copy(last);
      /* Use the global scope, but we need to add a 'global' namespace.  */
      if (!gscope) gscope = set_scope_to_global();
      /* note that this namespace is not the "unnamed" one,
	 and we don't use Setattr(nscope,"name", ""),
	 because the unnamed namespace is private */
      nscope = new_node("namespace");
      Setattr(nscope,"symtab", gscope);;
      nscope_inner = nscope;
      Delete(last);
      return base;
    }
    /* Try to locate the scope */
    ns = Swig_symbol_clookup(prefix,0);
    if (!ns) {
      Swig_error(cparse_file, cparse_line, "Undefined scope '%s'\n", SwigType_namestr(prefix));
      *errored = 1;
    } else {
      Symtab *nstab = Getattr(ns,"symtab");
      if (!nstab) {
	Swig_error(cparse_file, cparse_line, "'%s' is not defined as a valid scope.\n", SwigType_namestr(prefix));
	*errored = 1;
	ns = 0;
      } else {
	/* Check if the node scope is the current scope */
	String *tname = Swig_symbol_qualifiedscopename(0);
	String *nname = Swig_symbol_qualifiedscopename(nstab);
	if (tname && (Strcmp(tname,nname) == 0)) {
	  ns = 0;
	  cname = Copy(last);
	}
	Delete(tname);
	Delete(nname);
      }
      if (ns) {
	/* we will try to create a new node using the namespaces we
	   can find in the scope name */
	List *scopes = Swig_scopename_tolist(prefix);
	String *sname;
	Iterator si;

	for (si = First(scopes); si.item; si = Next(si)) {
	  Node *ns1,*ns2;
	  sname = si.item;
	  ns1 = Swig_symbol_clookup(sname,0);
	  assert(ns1);
	  if (Strcmp(nodeType(ns1),"namespace") == 0) {
	    if (Getattr(ns1,"alias")) {
	      ns1 = Getattr(ns1,"namespace");
	    }
	  } else {
	    /* now this last part is a class */
	    si = Next(si);
	    /*  or a nested class tree, which is unrolled here */
	    for (; si.item; si = Next(si)) {
	      if (si.item) {
		Printf(sname,"::%s",si.item);
	      }
	    }
	    /* we get the 'inner' class */
	    nscope_inner = Swig_symbol_clookup(sname,0);
	    /* set the scope to the inner class */
	    Swig_symbol_setscope(Getattr(nscope_inner,"symtab"));
	    /* save the last namespace prefix */
	    Delete(Namespaceprefix);
	    Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	    /* and return the node name, including the inner class prefix */
	    break;
	  }
	  /* here we just populate the namespace tree as usual */
	  ns2 = new_node("namespace");
	  Setattr(ns2,"name",sname);
	  Setattr(ns2,"symtab", Getattr(ns1,"symtab"));
	  add_symbols(ns2);
	  Swig_symbol_setscope(Getattr(ns1,"symtab"));
	  Delete(Namespaceprefix);
	  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	  if (nscope_inner) {
	    if (Getattr(nscope_inner,"symtab") != Getattr(ns2,"symtab")) {
	      appendChild(nscope_inner,ns2);
	      Delete(ns2);
	    }
	  }
	  nscope_inner = ns2;
	  if (!nscope) nscope = ns2;
	}
	cname = Copy(last);
	Delete(scopes);
      }
    }
    Delete(prefix);
  }
  Delete(last);

  return cname;
}
 
/* look for simple typedef name in typedef list */
static String *try_to_find_a_name_for_unnamed_structure(const String *storage, Node *decls) {
  String *name = 0;
  Node *n = decls;
  if (storage && Equal(storage, "typedef")) {
    for (; n; n = nextSibling(n)) {
      if (!Len(Getattr(n, "decl"))) {
	name = Copy(Getattr(n, "name"));
	break;
      }
    }
  }
  return name;
}

/* traverse copied tree segment, and update outer class links*/
static void update_nested_classes(Node *n)
{
  Node *c = firstChild(n);
  while (c) {
    if (Getattr(c, "nested:outer"))
      Setattr(c, "nested:outer", n);
    update_nested_classes(c);
    c = nextSibling(c);
  }
}

/* -----------------------------------------------------------------------------
 * nested_forward_declaration()
 * 
 * Nested struct handling for C++ code if the nested classes are disabled.
 * Create the nested class/struct/union as a forward declaration.
 * ----------------------------------------------------------------------------- */

static Node *nested_forward_declaration(const String *storage, const String *kind, String *sname, String *name, Node *cpp_opt_declarators) {
  Node *nn = 0;

  if (sname) {
    /* Add forward declaration of the nested type */
    Node *n = new_node("classforward");
    Setattr(n, "kind", kind);
    Setattr(n, "name", sname);
    Setattr(n, "storage", storage);
    Setattr(n, "sym:weak", "1");
    SetFlag(n, "nested:forward");
    add_symbols(n);
    nn = n;
  }

  /* Add any variable instances. Also add in any further typedefs of the nested type.
     Note that anonymous typedefs (eg typedef struct {...} a, b;) are treated as class forward declarations */
  if (cpp_opt_declarators) {
    int storage_typedef = (storage && Equal(storage, "typedef"));
    int variable_of_anonymous_type = !sname && !storage_typedef;
    if (!variable_of_anonymous_type) {
      int anonymous_typedef = !sname && storage_typedef;
      Node *n = cpp_opt_declarators;
      SwigType *type = name;
      while (n) {
	Setattr(n, "type", type);
	Setattr(n, "storage", storage);
	if (anonymous_typedef) {
	  Setattr(n, "nodeType", "classforward");
	  Setattr(n, "sym:weak", "1");
	}
	n = nextSibling(n);
      }
      add_symbols(cpp_opt_declarators);

      if (nn) {
	set_nextSibling(nn, cpp_opt_declarators);
      } else {
	nn = cpp_opt_declarators;
      }
    }
  }

  if (!currentOuterClass || !GetFlag(currentOuterClass, "nested")) {
    if (nn && Equal(nodeType(nn), "classforward")) {
      Node *n = nn;
      if (!GetFlag(n, "feature:ignore")) {
	SWIG_WARN_NODE_BEGIN(n);
	Swig_warning(WARN_PARSE_NAMED_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (%s ignored)\n", kind, SwigType_namestr(sname ? sname : name));
	SWIG_WARN_NODE_END(n);
      }
    } else {
      Swig_warning(WARN_PARSE_UNNAMED_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", kind);
    }
  }

  return nn;
}


Node *Swig_cparse(File *f) {
  scanner_file(f);
  top = 0;
  if (yyparse() == 2) {
    Swig_error(cparse_file, cparse_line, "Parser exceeded stack depth or ran out of memory\n");
    Exit(EXIT_FAILURE);
  }
  return (Node *)top;
}

static void single_new_feature(const char *featurename, String *val, Hash *featureattribs, char *declaratorid, SwigType *type, ParmList *declaratorparms, String *qualifier) {
  String *fname;
  String *name;
  String *fixname;
  SwigType *t = Copy(type);

  /* Printf(stdout, "single_new_feature: [%s] [%s] [%s] [%s] [%s] [%s]\n", featurename, val, declaratorid, t, ParmList_str_defaultargs(declaratorparms), qualifier); */

  fname = NewStringf("feature:%s",featurename);
  if (declaratorid) {
    fixname = feature_identifier_fix(declaratorid);
  } else {
    fixname = NewStringEmpty();
  }
  if (Namespaceprefix) {
    name = NewStringf("%s::%s",Namespaceprefix, fixname);
  } else {
    name = fixname;
  }

  if (declaratorparms) Setmeta(val,"parms",declaratorparms);
  if (!Len(t)) t = 0;
  if (t) {
    if (qualifier) SwigType_push(t,qualifier);
    if (SwigType_isfunction(t)) {
      SwigType *decl = SwigType_pop_function(t);
      if (SwigType_ispointer(t)) {
	String *nname = NewStringf("*%s",name);
	Swig_feature_set(Swig_cparse_features(), nname, decl, fname, val, featureattribs);
	Delete(nname);
      } else {
	Swig_feature_set(Swig_cparse_features(), name, decl, fname, val, featureattribs);
      }
      Delete(decl);
    } else if (SwigType_ispointer(t)) {
      String *nname = NewStringf("*%s",name);
      Swig_feature_set(Swig_cparse_features(),nname,0,fname,val, featureattribs);
      Delete(nname);
    }
  } else {
    /* Global feature, that is, feature not associated with any particular symbol */
    Swig_feature_set(Swig_cparse_features(),name,0,fname,val, featureattribs);
  }
  Delete(fname);
  Delete(name);
}

/* Add a new feature to the Hash. Additional features are added if the feature has a parameter list (declaratorparms)
 * and one or more of the parameters have a default argument. An extra feature is added for each defaulted parameter,
 * simulating the equivalent overloaded method. */
static void new_feature(const char *featurename, String *val, Hash *featureattribs, char *declaratorid, SwigType *type, ParmList *declaratorparms, String *qualifier) {

  ParmList *declparms = declaratorparms;

  /* remove the { and } braces if the noblock attribute is set */
  String *newval = remove_block(featureattribs, val);
  val = newval ? newval : val;

  /* Add the feature */
  single_new_feature(featurename, val, featureattribs, declaratorid, type, declaratorparms, qualifier);

  /* Add extra features if there are default parameters in the parameter list */
  if (type) {
    while (declparms) {
      if (ParmList_has_defaultargs(declparms)) {

        /* Create a parameter list for the new feature by copying all
           but the last (defaulted) parameter */
        ParmList* newparms = CopyParmListMax(declparms, ParmList_len(declparms)-1);

        /* Create new declaration - with the last parameter removed */
        SwigType *newtype = Copy(type);
        Delete(SwigType_pop_function(newtype)); /* remove the old parameter list from newtype */
        SwigType_add_function(newtype,newparms);

        single_new_feature(featurename, Copy(val), featureattribs, declaratorid, newtype, newparms, qualifier);
        declparms = newparms;
      } else {
        declparms = 0;
      }
    }
  }
}

/* check if a function declaration is a plain C object */
static int is_cfunction(Node *n) {
  if (!cparse_cplusplus || cparse_externc)
    return 1;
  if (Swig_storage_isexternc(n)) {
    return 1;
  }
  return 0;
}

/* If the Node is a function with parameters, check to see if any of the parameters
 * have default arguments. If so create a new function for each defaulted argument. 
 * The additional functions form a linked list of nodes with the head being the original Node n. */
static void default_arguments(Node *n) {
  Node *function = n;

  if (function) {
    ParmList *varargs = Getattr(function,"feature:varargs");
    if (varargs) {
      /* Handles the %varargs directive by looking for "feature:varargs" and 
       * substituting ... with an alternative set of arguments.  */
      Parm     *p = Getattr(function,"parms");
      Parm     *pp = 0;
      while (p) {
	SwigType *t = Getattr(p,"type");
	if (Strcmp(t,"v(...)") == 0) {
	  if (pp) {
	    ParmList *cv = Copy(varargs);
	    set_nextSibling(pp,cv);
	    Delete(cv);
	  } else {
	    ParmList *cv =  Copy(varargs);
	    Setattr(function,"parms", cv);
	    Delete(cv);
	  }
	  break;
	}
	pp = p;
	p = nextSibling(p);
      }
    }

    /* Do not add in functions if kwargs is being used or if user wants old default argument wrapping
       (one wrapped method per function irrespective of number of default arguments) */
    if (compact_default_args 
	|| is_cfunction(function) 
	|| GetFlag(function,"feature:compactdefaultargs") 
	|| (GetFlag(function,"feature:kwargs") && kwargs_supported)) {
      ParmList *p = Getattr(function,"parms");
      if (p) 
        Setattr(p,"compactdefargs", "1"); /* mark parameters for special handling */
      function = 0; /* don't add in extra methods */
    }
  }

  while (function) {
    ParmList *parms = Getattr(function,"parms");
    if (ParmList_has_defaultargs(parms)) {

      /* Create a parameter list for the new function by copying all
         but the last (defaulted) parameter */
      ParmList* newparms = CopyParmListMax(parms,ParmList_len(parms)-1);

      /* Create new function and add to symbol table */
      {
	SwigType *ntype = Copy(nodeType(function));
	char *cntype = Char(ntype);
        Node *new_function = new_node(ntype);
        SwigType *decl = Copy(Getattr(function,"decl"));
        int constqualifier = SwigType_isconst(decl);
	String *ccode = Copy(Getattr(function,"code"));
	String *cstorage = Copy(Getattr(function,"storage"));
	String *cvalue = Copy(Getattr(function,"value"));
	SwigType *ctype = Copy(Getattr(function,"type"));
	String *cthrow = Copy(Getattr(function,"throw"));

        Delete(SwigType_pop_function(decl)); /* remove the old parameter list from decl */
        SwigType_add_function(decl,newparms);
        if (constqualifier)
          SwigType_add_qualifier(decl,"const");

        Setattr(new_function,"name", Getattr(function,"name"));
        Setattr(new_function,"code", ccode);
        Setattr(new_function,"decl", decl);
        Setattr(new_function,"parms", newparms);
        Setattr(new_function,"storage", cstorage);
        Setattr(new_function,"value", cvalue);
        Setattr(new_function,"type", ctype);
        Setattr(new_function,"throw", cthrow);

	Delete(ccode);
	Delete(cstorage);
	Delete(cvalue);
	Delete(ctype);
	Delete(cthrow);
	Delete(decl);

        {
          Node *throws = Getattr(function,"throws");
	  ParmList *pl = CopyParmList(throws);
          if (throws) Setattr(new_function,"throws",pl);
	  Delete(pl);
        }

        /* copy specific attributes for global (or in a namespace) template functions - these are not class template methods */
        if (strcmp(cntype,"template") == 0) {
          Node *templatetype = Getattr(function,"templatetype");
          Node *symtypename = Getattr(function,"sym:typename");
          Parm *templateparms = Getattr(function,"templateparms");
          if (templatetype) {
	    Node *tmp = Copy(templatetype);
	    Setattr(new_function,"templatetype",tmp);
	    Delete(tmp);
	  }
          if (symtypename) {
	    Node *tmp = Copy(symtypename);
	    Setattr(new_function,"sym:typename",tmp);
	    Delete(tmp);
	  }
          if (templateparms) {
	    Parm *tmp = CopyParmList(templateparms);
	    Setattr(new_function,"templateparms",tmp);
	    Delete(tmp);
	  }
        } else if (strcmp(cntype,"constructor") == 0) {
          /* only copied for constructors as this is not a user defined feature - it is hard coded in the parser */
          if (GetFlag(function,"feature:new")) SetFlag(new_function,"feature:new");
        }

        add_symbols(new_function);
        /* mark added functions as ones with overloaded parameters and point to the parsed method */
        Setattr(new_function,"defaultargs", n);

        /* Point to the new function, extending the linked list */
        set_nextSibling(function, new_function);
	Delete(new_function);
        function = new_function;
	
	Delete(ntype);
      }
    } else {
      function = 0;
    }
  }
}

/* -----------------------------------------------------------------------------
 * mark_nodes_as_extend()
 *
 * Used by the %extend to mark subtypes with "feature:extend".
 * template instances declared within %extend are skipped
 * ----------------------------------------------------------------------------- */

static void mark_nodes_as_extend(Node *n) {
  for (; n; n = nextSibling(n)) {
    if (Getattr(n, "template") && Strcmp(nodeType(n), "class") == 0)
      continue;
    /* Fix me: extend is not a feature. Replace with isextendmember? */
    Setattr(n, "feature:extend", "1");
    mark_nodes_as_extend(firstChild(n));
  }
}

/* -----------------------------------------------------------------------------
 * add_qualifier_to_declarator()
 *
 * Normally the qualifier is pushed on to the front of the type.
 * Adding a qualifier to a pointer to member function is a special case.
 * For example       : typedef double (Cls::*pmf)(void) const;
 * The qualifier is  : q(const).
 * The declarator is : m(Cls).f(void).
 * We need           : m(Cls).q(const).f(void).
 * ----------------------------------------------------------------------------- */

static String *add_qualifier_to_declarator(SwigType *type, SwigType *qualifier) {
  int is_pointer_to_member_function = 0;
  String *decl = Copy(type);
  String *poppedtype = NewString("");
  assert(qualifier);

  while (decl) {
    if (SwigType_ismemberpointer(decl)) {
      String *memberptr = SwigType_pop(decl);
      if (SwigType_isfunction(decl)) {
	is_pointer_to_member_function = 1;
	SwigType_push(decl, qualifier);
	SwigType_push(decl, memberptr);
	Insert(decl, 0, poppedtype);
	Delete(memberptr);
	break;
      } else {
	Append(poppedtype, memberptr);
      }
      Delete(memberptr);
    } else {
      String *popped = SwigType_pop(decl);
      if (!popped)
	break;
      Append(poppedtype, popped);
      Delete(popped);
    }
  }

  if (!is_pointer_to_member_function) {
    Delete(decl);
    decl = Copy(type);
    SwigType_push(decl, qualifier);
  }

  Delete(poppedtype);
  return decl;
}


#line 1646 "CParse/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* END  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_HBLOCK = 4,                     /* HBLOCK  */
  YYSYMBOL_POUND = 5,                      /* POUND  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_WSTRING = 7,                    /* WSTRING  */
  YYSYMBOL_INCLUDE = 8,                    /* INCLUDE  */
  YYSYMBOL_IMPORT = 9,                     /* IMPORT  */
  YYSYMBOL_INSERT = 10,                    /* INSERT  */
  YYSYMBOL_CHARCONST = 11,                 /* CHARCONST  */
  YYSYMBOL_WCHARCONST = 12,                /* WCHARCONST  */
  YYSYMBOL_NUM_INT = 13,                   /* NUM_INT  */
  YYSYMBOL_NUM_DOUBLE = 14,                /* NUM_DOUBLE  */
  YYSYMBOL_NUM_FLOAT = 15,                 /* NUM_FLOAT  */
  YYSYMBOL_NUM_LONGDOUBLE = 16,            /* NUM_LONGDOUBLE  */
  YYSYMBOL_NUM_UNSIGNED = 17,              /* NUM_UNSIGNED  */
  YYSYMBOL_NUM_LONG = 18,                  /* NUM_LONG  */
  YYSYMBOL_NUM_ULONG = 19,                 /* NUM_ULONG  */
  YYSYMBOL_NUM_LONGLONG = 20,              /* NUM_LONGLONG  */
  YYSYMBOL_NUM_ULONGLONG = 21,             /* NUM_ULONGLONG  */
  YYSYMBOL_NUM_BOOL = 22,                  /* NUM_BOOL  */
  YYSYMBOL_TYPEDEF = 23,                   /* TYPEDEF  */
  YYSYMBOL_TYPE_INT = 24,                  /* TYPE_INT  */
  YYSYMBOL_TYPE_UNSIGNED = 25,             /* TYPE_UNSIGNED  */
  YYSYMBOL_TYPE_SHORT = 26,                /* TYPE_SHORT  */
  YYSYMBOL_TYPE_LONG = 27,                 /* TYPE_LONG  */
  YYSYMBOL_TYPE_FLOAT = 28,                /* TYPE_FLOAT  */
  YYSYMBOL_TYPE_DOUBLE = 29,               /* TYPE_DOUBLE  */
  YYSYMBOL_TYPE_CHAR = 30,                 /* TYPE_CHAR  */
  YYSYMBOL_TYPE_WCHAR = 31,                /* TYPE_WCHAR  */
  YYSYMBOL_TYPE_VOID = 32,                 /* TYPE_VOID  */
  YYSYMBOL_TYPE_SIGNED = 33,               /* TYPE_SIGNED  */
  YYSYMBOL_TYPE_BOOL = 34,                 /* TYPE_BOOL  */
  YYSYMBOL_TYPE_COMPLEX = 35,              /* TYPE_COMPLEX  */
  YYSYMBOL_TYPE_RAW = 36,                  /* TYPE_RAW  */
  YYSYMBOL_TYPE_NON_ISO_INT8 = 37,         /* TYPE_NON_ISO_INT8  */
  YYSYMBOL_TYPE_NON_ISO_INT16 = 38,        /* TYPE_NON_ISO_INT16  */
  YYSYMBOL_TYPE_NON_ISO_INT32 = 39,        /* TYPE_NON_ISO_INT32  */
  YYSYMBOL_TYPE_NON_ISO_INT64 = 40,        /* TYPE_NON_ISO_INT64  */
  YYSYMBOL_LPAREN = 41,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 42,                    /* RPAREN  */
  YYSYMBOL_COMMA = 43,                     /* COMMA  */
  YYSYMBOL_SEMI = 44,                      /* SEMI  */
  YYSYMBOL_EXTERN = 45,                    /* EXTERN  */
  YYSYMBOL_LBRACE = 46,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 47,                    /* RBRACE  */
  YYSYMBOL_PERIOD = 48,                    /* PERIOD  */
  YYSYMBOL_ELLIPSIS = 49,                  /* ELLIPSIS  */
  YYSYMBOL_CONST_QUAL = 50,                /* CONST_QUAL  */
  YYSYMBOL_VOLATILE = 51,                  /* VOLATILE  */
  YYSYMBOL_REGISTER = 52,                  /* REGISTER  */
  YYSYMBOL_STRUCT = 53,                    /* STRUCT  */
  YYSYMBOL_UNION = 54,                     /* UNION  */
  YYSYMBOL_EQUAL = 55,                     /* EQUAL  */
  YYSYMBOL_SIZEOF = 56,                    /* SIZEOF  */
  YYSYMBOL_ALIGNOF = 57,                   /* ALIGNOF  */
  YYSYMBOL_MODULE = 58,                    /* MODULE  */
  YYSYMBOL_LBRACKET = 59,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 60,                  /* RBRACKET  */
  YYSYMBOL_BEGINFILE = 61,                 /* BEGINFILE  */
  YYSYMBOL_ENDOFFILE = 62,                 /* ENDOFFILE  */
  YYSYMBOL_CONSTANT = 63,                  /* CONSTANT  */
  YYSYMBOL_RENAME = 64,                    /* RENAME  */
  YYSYMBOL_NAMEWARN = 65,                  /* NAMEWARN  */
  YYSYMBOL_EXTEND = 66,                    /* EXTEND  */
  YYSYMBOL_PRAGMA = 67,                    /* PRAGMA  */
  YYSYMBOL_FEATURE = 68,                   /* FEATURE  */
  YYSYMBOL_VARARGS = 69,                   /* VARARGS  */
  YYSYMBOL_ENUM = 70,                      /* ENUM  */
  YYSYMBOL_CLASS = 71,                     /* CLASS  */
  YYSYMBOL_TYPENAME = 72,                  /* TYPENAME  */
  YYSYMBOL_PRIVATE = 73,                   /* PRIVATE  */
  YYSYMBOL_PUBLIC = 74,                    /* PUBLIC  */
  YYSYMBOL_PROTECTED = 75,                 /* PROTECTED  */
  YYSYMBOL_COLON = 76,                     /* COLON  */
  YYSYMBOL_STATIC = 77,                    /* STATIC  */
  YYSYMBOL_VIRTUAL = 78,                   /* VIRTUAL  */
  YYSYMBOL_FRIEND = 79,                    /* FRIEND  */
  YYSYMBOL_THROW = 80,                     /* THROW  */
  YYSYMBOL_CATCH = 81,                     /* CATCH  */
  YYSYMBOL_EXPLICIT = 82,                  /* EXPLICIT  */
  YYSYMBOL_STATIC_ASSERT = 83,             /* STATIC_ASSERT  */
  YYSYMBOL_CONSTEXPR = 84,                 /* CONSTEXPR  */
  YYSYMBOL_THREAD_LOCAL = 85,              /* THREAD_LOCAL  */
  YYSYMBOL_DECLTYPE = 86,                  /* DECLTYPE  */
  YYSYMBOL_AUTO = 87,                      /* AUTO  */
  YYSYMBOL_NOEXCEPT = 88,                  /* NOEXCEPT  */
  YYSYMBOL_OVERRIDE = 89,                  /* OVERRIDE  */
  YYSYMBOL_FINAL = 90,                     /* FINAL  */
  YYSYMBOL_USING = 91,                     /* USING  */
  YYSYMBOL_NAMESPACE = 92,                 /* NAMESPACE  */
  YYSYMBOL_NATIVE = 93,                    /* NATIVE  */
  YYSYMBOL_INLINE = 94,                    /* INLINE  */
  YYSYMBOL_TYPEMAP = 95,                   /* TYPEMAP  */
  YYSYMBOL_ECHO = 96,                      /* ECHO  */
  YYSYMBOL_APPLY = 97,                     /* APPLY  */
  YYSYMBOL_CLEAR = 98,                     /* CLEAR  */
  YYSYMBOL_SWIGTEMPLATE = 99,              /* SWIGTEMPLATE  */
  YYSYMBOL_FRAGMENT = 100,                 /* FRAGMENT  */
  YYSYMBOL_WARN = 101,                     /* WARN  */
  YYSYMBOL_LESSTHAN = 102,                 /* LESSTHAN  */
  YYSYMBOL_GREATERTHAN = 103,              /* GREATERTHAN  */
  YYSYMBOL_DELETE_KW = 104,                /* DELETE_KW  */
  YYSYMBOL_DEFAULT = 105,                  /* DEFAULT  */
  YYSYMBOL_LESSTHANOREQUALTO = 106,        /* LESSTHANOREQUALTO  */
  YYSYMBOL_GREATERTHANOREQUALTO = 107,     /* GREATERTHANOREQUALTO  */
  YYSYMBOL_EQUALTO = 108,                  /* EQUALTO  */
  YYSYMBOL_NOTEQUALTO = 109,               /* NOTEQUALTO  */
  YYSYMBOL_LESSEQUALGREATER = 110,         /* LESSEQUALGREATER  */
  YYSYMBOL_ARROW = 111,                    /* ARROW  */
  YYSYMBOL_QUESTIONMARK = 112,             /* QUESTIONMARK  */
  YYSYMBOL_TYPES = 113,                    /* TYPES  */
  YYSYMBOL_PARMS = 114,                    /* PARMS  */
  YYSYMBOL_NONID = 115,                    /* NONID  */
  YYSYMBOL_DSTAR = 116,                    /* DSTAR  */
  YYSYMBOL_DCNOT = 117,                    /* DCNOT  */
  YYSYMBOL_TEMPLATE = 118,                 /* TEMPLATE  */
  YYSYMBOL_OPERATOR = 119,                 /* OPERATOR  */
  YYSYMBOL_CONVERSIONOPERATOR = 120,       /* CONVERSIONOPERATOR  */
  YYSYMBOL_PARSETYPE = 121,                /* PARSETYPE  */
  YYSYMBOL_PARSEPARM = 122,                /* PARSEPARM  */
  YYSYMBOL_PARSEPARMS = 123,               /* PARSEPARMS  */
  YYSYMBOL_DOXYGENSTRING = 124,            /* DOXYGENSTRING  */
  YYSYMBOL_DOXYGENPOSTSTRING = 125,        /* DOXYGENPOSTSTRING  */
  YYSYMBOL_LOR = 126,                      /* LOR  */
  YYSYMBOL_LAND = 127,                     /* LAND  */
  YYSYMBOL_OR = 128,                       /* OR  */
  YYSYMBOL_XOR = 129,                      /* XOR  */
  YYSYMBOL_AND = 130,                      /* AND  */
  YYSYMBOL_LSHIFT = 131,                   /* LSHIFT  */
  YYSYMBOL_RSHIFT = 132,                   /* RSHIFT  */
  YYSYMBOL_PLUS = 133,                     /* PLUS  */
  YYSYMBOL_MINUS = 134,                    /* MINUS  */
  YYSYMBOL_STAR = 135,                     /* STAR  */
  YYSYMBOL_SLASH = 136,                    /* SLASH  */
  YYSYMBOL_MODULO = 137,                   /* MODULO  */
  YYSYMBOL_UMINUS = 138,                   /* UMINUS  */
  YYSYMBOL_NOT = 139,                      /* NOT  */
  YYSYMBOL_LNOT = 140,                     /* LNOT  */
  YYSYMBOL_CAST = 141,                     /* CAST  */
  YYSYMBOL_DCOLON = 142,                   /* DCOLON  */
  YYSYMBOL_YYACCEPT = 143,                 /* $accept  */
  YYSYMBOL_program = 144,                  /* program  */
  YYSYMBOL_interface = 145,                /* interface  */
  YYSYMBOL_declaration = 146,              /* declaration  */
  YYSYMBOL_swig_directive = 147,           /* swig_directive  */
  YYSYMBOL_extend_directive = 148,         /* extend_directive  */
  YYSYMBOL_149_1 = 149,                    /* $@1  */
  YYSYMBOL_apply_directive = 150,          /* apply_directive  */
  YYSYMBOL_clear_directive = 151,          /* clear_directive  */
  YYSYMBOL_constant_directive = 152,       /* constant_directive  */
  YYSYMBOL_echo_directive = 153,           /* echo_directive  */
  YYSYMBOL_stringtype = 154,               /* stringtype  */
  YYSYMBOL_fname = 155,                    /* fname  */
  YYSYMBOL_fragment_directive = 156,       /* fragment_directive  */
  YYSYMBOL_include_directive = 157,        /* include_directive  */
  YYSYMBOL_158_2 = 158,                    /* @2  */
  YYSYMBOL_includetype = 159,              /* includetype  */
  YYSYMBOL_inline_directive = 160,         /* inline_directive  */
  YYSYMBOL_insert_directive = 161,         /* insert_directive  */
  YYSYMBOL_module_directive = 162,         /* module_directive  */
  YYSYMBOL_native_directive = 163,         /* native_directive  */
  YYSYMBOL_pragma_directive = 164,         /* pragma_directive  */
  YYSYMBOL_pragma_arg = 165,               /* pragma_arg  */
  YYSYMBOL_pragma_lang = 166,              /* pragma_lang  */
  YYSYMBOL_rename_directive = 167,         /* rename_directive  */
  YYSYMBOL_rename_namewarn = 168,          /* rename_namewarn  */
  YYSYMBOL_feature_directive = 169,        /* feature_directive  */
  YYSYMBOL_stringbracesemi = 170,          /* stringbracesemi  */
  YYSYMBOL_featattr = 171,                 /* featattr  */
  YYSYMBOL_varargs_directive = 172,        /* varargs_directive  */
  YYSYMBOL_varargs_parms = 173,            /* varargs_parms  */
  YYSYMBOL_typemap_directive = 174,        /* typemap_directive  */
  YYSYMBOL_typemap_type = 175,             /* typemap_type  */
  YYSYMBOL_tm_list = 176,                  /* tm_list  */
  YYSYMBOL_tm_list_builder = 177,          /* tm_list_builder  */
  YYSYMBOL_typemap_parm = 178,             /* typemap_parm  */
  YYSYMBOL_types_directive = 179,          /* types_directive  */
  YYSYMBOL_template_directive = 180,       /* template_directive  */
  YYSYMBOL_warn_directive = 181,           /* warn_directive  */
  YYSYMBOL_c_declaration = 182,            /* c_declaration  */
  YYSYMBOL_183_3 = 183,                    /* $@3  */
  YYSYMBOL_c_decl = 184,                   /* c_decl  */
  YYSYMBOL_c_decl_tail = 185,              /* c_decl_tail  */
  YYSYMBOL_initializer = 186,              /* initializer  */
  YYSYMBOL_cpp_alternate_rettype = 187,    /* cpp_alternate_rettype  */
  YYSYMBOL_cpp_lambda_decl = 188,          /* cpp_lambda_decl  */
  YYSYMBOL_lambda_introducer = 189,        /* lambda_introducer  */
  YYSYMBOL_lambda_template = 190,          /* lambda_template  */
  YYSYMBOL_lambda_body = 191,              /* lambda_body  */
  YYSYMBOL_lambda_tail = 192,              /* lambda_tail  */
  YYSYMBOL_193_4 = 193,                    /* $@4  */
  YYSYMBOL_c_enum_key = 194,               /* c_enum_key  */
  YYSYMBOL_c_enum_inherit = 195,           /* c_enum_inherit  */
  YYSYMBOL_c_enum_forward_decl = 196,      /* c_enum_forward_decl  */
  YYSYMBOL_c_enum_decl = 197,              /* c_enum_decl  */
  YYSYMBOL_c_constructor_decl = 198,       /* c_constructor_decl  */
  YYSYMBOL_cpp_declaration = 199,          /* cpp_declaration  */
  YYSYMBOL_cpp_class_decl = 200,           /* cpp_class_decl  */
  YYSYMBOL_201_5 = 201,                    /* @5  */
  YYSYMBOL_202_6 = 202,                    /* @6  */
  YYSYMBOL_cpp_opt_declarators = 203,      /* cpp_opt_declarators  */
  YYSYMBOL_cpp_forward_class_decl = 204,   /* cpp_forward_class_decl  */
  YYSYMBOL_cpp_template_decl = 205,        /* cpp_template_decl  */
  YYSYMBOL_206_7 = 206,                    /* $@7  */
  YYSYMBOL_cpp_template_possible = 207,    /* cpp_template_possible  */
  YYSYMBOL_template_parms = 208,           /* template_parms  */
  YYSYMBOL_template_parms_builder = 209,   /* template_parms_builder  */
  YYSYMBOL_templateparameter = 210,        /* templateparameter  */
  YYSYMBOL_cpp_using_decl = 211,           /* cpp_using_decl  */
  YYSYMBOL_cpp_namespace_decl = 212,       /* cpp_namespace_decl  */
  YYSYMBOL_213_8 = 213,                    /* @8  */
  YYSYMBOL_214_9 = 214,                    /* @9  */
  YYSYMBOL_cpp_members = 215,              /* cpp_members  */
  YYSYMBOL_cpp_members_builder = 216,      /* cpp_members_builder  */
  YYSYMBOL_cpp_member_no_dox = 217,        /* cpp_member_no_dox  */
  YYSYMBOL_cpp_member = 218,               /* cpp_member  */
  YYSYMBOL_219_10 = 219,                   /* $@10  */
  YYSYMBOL_cpp_constructor_decl = 220,     /* cpp_constructor_decl  */
  YYSYMBOL_cpp_destructor_decl = 221,      /* cpp_destructor_decl  */
  YYSYMBOL_cpp_conversion_operator = 222,  /* cpp_conversion_operator  */
  YYSYMBOL_cpp_catch_decl = 223,           /* cpp_catch_decl  */
  YYSYMBOL_cpp_static_assert = 224,        /* cpp_static_assert  */
  YYSYMBOL_cpp_protection_decl = 225,      /* cpp_protection_decl  */
  YYSYMBOL_cpp_swig_directive = 226,       /* cpp_swig_directive  */
  YYSYMBOL_cpp_vend = 227,                 /* cpp_vend  */
  YYSYMBOL_anonymous_bitfield = 228,       /* anonymous_bitfield  */
  YYSYMBOL_anon_bitfield_type = 229,       /* anon_bitfield_type  */
  YYSYMBOL_storage_class = 230,            /* storage_class  */
  YYSYMBOL_storage_class_list = 231,       /* storage_class_list  */
  YYSYMBOL_storage_class_raw = 232,        /* storage_class_raw  */
  YYSYMBOL_parms = 233,                    /* parms  */
  YYSYMBOL_rawparms = 234,                 /* rawparms  */
  YYSYMBOL_parm_no_dox = 235,              /* parm_no_dox  */
  YYSYMBOL_parm = 236,                     /* parm  */
  YYSYMBOL_valparms = 237,                 /* valparms  */
  YYSYMBOL_valparms_builder = 238,         /* valparms_builder  */
  YYSYMBOL_valparm = 239,                  /* valparm  */
  YYSYMBOL_def_args = 240,                 /* def_args  */
  YYSYMBOL_parameter_declarator = 241,     /* parameter_declarator  */
  YYSYMBOL_plain_declarator = 242,         /* plain_declarator  */
  YYSYMBOL_declarator = 243,               /* declarator  */
  YYSYMBOL_notso_direct_declarator = 244,  /* notso_direct_declarator  */
  YYSYMBOL_direct_declarator = 245,        /* direct_declarator  */
  YYSYMBOL_abstract_declarator = 246,      /* abstract_declarator  */
  YYSYMBOL_direct_abstract_declarator = 247, /* direct_abstract_declarator  */
  YYSYMBOL_pointer = 248,                  /* pointer  */
  YYSYMBOL_cv_ref_qualifier = 249,         /* cv_ref_qualifier  */
  YYSYMBOL_ref_qualifier = 250,            /* ref_qualifier  */
  YYSYMBOL_type_qualifier = 251,           /* type_qualifier  */
  YYSYMBOL_type_qualifier_raw = 252,       /* type_qualifier_raw  */
  YYSYMBOL_type = 253,                     /* type  */
  YYSYMBOL_rawtype = 254,                  /* rawtype  */
  YYSYMBOL_type_right = 255,               /* type_right  */
  YYSYMBOL_decltype = 256,                 /* decltype  */
  YYSYMBOL_257_11 = 257,                   /* @11  */
  YYSYMBOL_decltypeexpr = 258,             /* decltypeexpr  */
  YYSYMBOL_primitive_type = 259,           /* primitive_type  */
  YYSYMBOL_primitive_type_list = 260,      /* primitive_type_list  */
  YYSYMBOL_type_specifier = 261,           /* type_specifier  */
  YYSYMBOL_definetype = 262,               /* definetype  */
  YYSYMBOL_default_delete = 263,           /* default_delete  */
  YYSYMBOL_deleted_definition = 264,       /* deleted_definition  */
  YYSYMBOL_explicit_default = 265,         /* explicit_default  */
  YYSYMBOL_ename = 266,                    /* ename  */
  YYSYMBOL_constant_directives = 267,      /* constant_directives  */
  YYSYMBOL_optional_ignored_defines = 268, /* optional_ignored_defines  */
  YYSYMBOL_enumlist = 269,                 /* enumlist  */
  YYSYMBOL_enumlist_item = 270,            /* enumlist_item  */
  YYSYMBOL_edecl_with_dox = 271,           /* edecl_with_dox  */
  YYSYMBOL_edecl = 272,                    /* edecl  */
  YYSYMBOL_etype = 273,                    /* etype  */
  YYSYMBOL_expr = 274,                     /* expr  */
  YYSYMBOL_exprmem = 275,                  /* exprmem  */
  YYSYMBOL_exprsimple = 276,               /* exprsimple  */
  YYSYMBOL_valexpr = 277,                  /* valexpr  */
  YYSYMBOL_exprnum = 278,                  /* exprnum  */
  YYSYMBOL_exprcompound = 279,             /* exprcompound  */
  YYSYMBOL_variadic_opt = 280,             /* variadic_opt  */
  YYSYMBOL_inherit = 281,                  /* inherit  */
  YYSYMBOL_raw_inherit = 282,              /* raw_inherit  */
  YYSYMBOL_283_12 = 283,                   /* $@12  */
  YYSYMBOL_base_list = 284,                /* base_list  */
  YYSYMBOL_base_specifier = 285,           /* base_specifier  */
  YYSYMBOL_286_13 = 286,                   /* @13  */
  YYSYMBOL_287_14 = 287,                   /* @14  */
  YYSYMBOL_access_specifier = 288,         /* access_specifier  */
  YYSYMBOL_templcpptype = 289,             /* templcpptype  */
  YYSYMBOL_cpptype = 290,                  /* cpptype  */
  YYSYMBOL_classkey = 291,                 /* classkey  */
  YYSYMBOL_classkeyopt = 292,              /* classkeyopt  */
  YYSYMBOL_opt_virtual = 293,              /* opt_virtual  */
  YYSYMBOL_virt_specifier_seq = 294,       /* virt_specifier_seq  */
  YYSYMBOL_virt_specifier_seq_opt = 295,   /* virt_specifier_seq_opt  */
  YYSYMBOL_class_virt_specifier_opt = 296, /* class_virt_specifier_opt  */
  YYSYMBOL_exception_specification = 297,  /* exception_specification  */
  YYSYMBOL_qualifiers_exception_specification = 298, /* qualifiers_exception_specification  */
  YYSYMBOL_cpp_const = 299,                /* cpp_const  */
  YYSYMBOL_ctor_end = 300,                 /* ctor_end  */
  YYSYMBOL_ctor_initializer = 301,         /* ctor_initializer  */
  YYSYMBOL_mem_initializer_list = 302,     /* mem_initializer_list  */
  YYSYMBOL_mem_initializer = 303,          /* mem_initializer  */
  YYSYMBOL_less_valparms_greater = 304,    /* less_valparms_greater  */
  YYSYMBOL_identifier = 305,               /* identifier  */
  YYSYMBOL_idstring = 306,                 /* idstring  */
  YYSYMBOL_idstringopt = 307,              /* idstringopt  */
  YYSYMBOL_idcolon = 308,                  /* idcolon  */
  YYSYMBOL_idcolontail = 309,              /* idcolontail  */
  YYSYMBOL_idtemplate = 310,               /* idtemplate  */
  YYSYMBOL_idtemplatetemplate = 311,       /* idtemplatetemplate  */
  YYSYMBOL_idcolonnt = 312,                /* idcolonnt  */
  YYSYMBOL_idcolontailnt = 313,            /* idcolontailnt  */
  YYSYMBOL_string = 314,                   /* string  */
  YYSYMBOL_wstring = 315,                  /* wstring  */
  YYSYMBOL_stringbrace = 316,              /* stringbrace  */
  YYSYMBOL_options = 317,                  /* options  */
  YYSYMBOL_kwargs = 318,                   /* kwargs  */
  YYSYMBOL_kwargs_builder = 319,           /* kwargs_builder  */
  YYSYMBOL_stringnum = 320                 /* stringnum  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 1885 "./CParse/parser.y"


// Default-initialised instances of token types to avoid uninitialised fields.
// The compiler will initialise all fields to zero or NULL for us.

static const struct Decl default_decl;
static const struct Define default_dtype;

/* C++ decltype/auto type deduction. */
static SwigType *deduce_type(const struct Define *dtype) {
  Node *n;
  if (!dtype->val) return NULL;
  n = Swig_symbol_clookup(dtype->val, 0);
  if (n) {
    if (Strcmp(nodeType(n),"enumitem") == 0) {
      /* For an enumitem, the "type" attribute gives us the underlying integer
       * type - we want the "type" attribute from the enum itself, which is
       * "parentNode".
       */
      n = Getattr(n, "parentNode");
    }
    return Getattr(n, "type");
  } else if (dtype->type != T_AUTO && dtype->type != T_UNKNOWN) {
    /* Try to deduce the type from the T_* type code. */
    String *deduced_type = NewSwigType(dtype->type);
    if (Len(deduced_type) > 0) return deduced_type;
    Delete(deduced_type);
  }
  return NULL;
}

static Node *new_enum_node(SwigType *enum_base_type) {
  Node *n = new_node("enum");
  if (enum_base_type) {
    switch (SwigType_type(enum_base_type)) {
      case T_USER:
	// We get T_USER if the underlying type is a typedef.  Unfortunately we
	// aren't able to resolve a typedef at this point, so we have to assume
	// it's a typedef to an integral or boolean type.
	break;
      case T_BOOL:
      case T_SCHAR:
      case T_UCHAR:
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
      case T_LONG:
      case T_ULONG:
      case T_LONGLONG:
      case T_ULONGLONG:
      case T_CHAR:
      case T_WCHAR:
	break;
      default:
	Swig_error(cparse_file, cparse_line, "Underlying type of enum must be an integral type\n");
    }
    Setattr(n, "enumbase", enum_base_type);
  }
  return n;
}


#line 2064 "CParse/parser.c"


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  60
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   9369

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  143
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  178
/* YYNRULES -- Number of rules.  */
#define YYNRULES  639
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1162

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   397


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX) YY_CAST (yysymbol_kind_t, YYX)

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  1958,  1958,  1970,  1974,  1977,  1980,  1983,  1986,  1991,
    2000,  2005,  2014,  2019,  2020,  2021,  2022,  2023,  2032,  2048,
    2059,  2060,  2061,  2062,  2063,  2064,  2065,  2066,  2067,  2068,
    2069,  2070,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2084,
    2084,  2166,  2176,  2190,  2208,  2226,  2244,  2248,  2259,  2268,
    2281,  2288,  2292,  2303,  2313,  2328,  2341,  2341,  2402,  2403,
    2410,  2428,  2462,  2467,  2477,  2483,  2501,  2544,  2551,  2578,
    2584,  2591,  2592,  2595,  2596,  2603,  2649,  2695,  2706,  2709,
    2736,  2742,  2750,  2756,  2764,  2765,  2766,  2769,  2775,  2782,
    2818,  2819,  2853,  2872,  2882,  2897,  2919,  2924,  2927,  2938,
    2949,  2954,  2967,  2979,  3223,  3233,  3240,  3241,  3245,  3245,
    3278,  3284,  3294,  3306,  3313,  3396,  3444,  3509,  3551,  3568,
    3583,  3587,  3614,  3618,  3629,  3630,  3636,  3637,  3638,  3639,
    3643,  3644,  3645,  3649,  3653,  3658,  3663,  3674,  3680,  3686,
    3694,  3699,  3702,  3705,  3709,  3710,  3710,  3722,  3725,  3728,
    3737,  3740,  3747,  3770,  3799,  3897,  3954,  3955,  3956,  3957,
    3958,  3959,  3968,  3968,  4215,  4215,  4370,  4371,  4383,  4402,
    4402,  4606,  4612,  4618,  4624,  4630,  4631,  4632,  4633,  4636,
    4637,  4640,  4643,  4648,  4651,  4662,  4670,  4678,  4686,  4711,
    4722,  4732,  4763,  4763,  4821,  4821,  4843,  4870,  4873,  4878,
    4881,  4886,  4892,  4895,  4925,  4926,  4943,  4944,  4945,  4946,
    4947,  4948,  4949,  4950,  4951,  4952,  4953,  4954,  4955,  4956,
    4957,  4958,  4959,  4961,  4962,  4966,  4970,  4970,  4989,  5027,
    5068,  5089,  5112,  5136,  5161,  5186,  5194,  5201,  5208,  5216,
    5224,  5227,  5229,  5230,  5231,  5232,  5233,  5234,  5235,  5236,
    5239,  5243,  5248,  5255,  5258,  5259,  5260,  5261,  5263,  5269,
    5311,  5314,  5315,  5323,  5324,  5334,  5335,  5336,  5337,  5338,
    5339,  5340,  5347,  5359,  5360,  5364,  5369,  5375,  5382,  5389,
    5394,  5406,  5414,  5415,  5421,  5429,  5434,  5437,  5448,  5476,
    5486,  5489,  5497,  5503,  5509,  5515,  5521,  5529,  5552,  5569,
    5588,  5607,  5612,  5620,  5629,  5638,  5642,  5651,  5662,  5673,
    5685,  5695,  5709,  5718,  5728,  5738,  5748,  5760,  5772,  5785,
    5796,  5807,  5819,  5833,  5838,  5844,  5856,  5864,  5875,  5886,
    5897,  5916,  5922,  5935,  5943,  5950,  5957,  5968,  5980,  5991,
    6003,  6014,  6025,  6045,  6059,  6064,  6070,  6076,  6082,  6091,
    6100,  6101,  6110,  6119,  6125,  6131,  6136,  6142,  6150,  6161,
    6172,  6183,  6188,  6193,  6196,  6213,  6231,  6241,  6246,  6250,
    6255,  6262,  6265,  6270,  6277,  6281,  6287,  6291,  6297,  6298,
    6299,  6305,  6311,  6315,  6316,  6320,  6325,  6331,  6332,  6333,
    6334,  6335,  6337,  6340,  6343,  6346,  6346,  6363,  6366,  6379,
    6405,  6406,  6460,  6464,  6468,  6472,  6476,  6480,  6484,  6488,
    6492,  6496,  6500,  6504,  6508,  6512,  6518,  6519,  6522,  6523,
    6527,  6535,  6544,  6545,  6548,  6549,  6553,  6554,  6564,  6568,
    6573,  6579,  6589,  6600,  6611,  6616,  6621,  6622,  6628,  6636,
    6654,  6672,  6673,  6700,  6704,  6710,  6714,  6720,  6724,  6730,
    6734,  6743,  6744,  6745,  6751,  6757,  6763,  6775,  6783,  6791,
    6798,  6808,  6815,  6816,  6819,  6829,  6868,  6878,  6888,  6898,
    6909,  6920,  6941,  6964,  6965,  6966,  6967,  6968,  6969,  6970,
    6971,  6972,  6973,  6976,  6981,  6986,  6991,  6996,  7001,  7006,
    7011,  7016,  7021,  7026,  7031,  7036,  7041,  7050,  7061,  7066,
    7071,  7088,  7093,  7098,  7103,  7108,  7113,  7118,  7123,  7128,
    7133,  7138,  7143,  7148,  7153,  7162,  7172,  7177,  7182,  7188,
    7203,  7210,  7236,  7247,  7252,  7257,  7284,  7287,  7292,  7295,
    7295,  7296,  7299,  7316,  7325,  7325,  7344,  7344,  7363,  7364,
    7365,  7368,  7373,  7380,  7381,  7385,  7391,  7394,  7397,  7402,
    7403,  7406,  7407,  7410,  7413,  7416,  7419,  7424,  7425,  7430,
    7433,  7438,  7443,  7447,  7451,  7457,  7462,  7468,  7473,  7476,
    7483,  7484,  7489,  7499,  7509,  7515,  7521,  7527,  7541,  7542,
    7545,  7546,  7547,  7548,  7551,  7563,  7569,  7578,  7579,  7580,
    7583,  7584,  7585,  7588,  7589,  7592,  7596,  7600,  7603,  7606,
    7609,  7614,  7620,  7624,  7627,  7631,  7637,  7640,  7647,  7648,
    7656,  7660,  7664,  7667,  7670,  7673,  7679,  7683,  7686,  7690,
    7696,  7701,  7703,  7709,  7715,  7721,  7724,  7725,  7729,  7732,
    7745,  7749,  7754,  7761,  7770,  7775,  7782,  7787,  7796,  7797
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "END", "error", "\"invalid token\"", "ID", "HBLOCK", "POUND", "STRING",
  "WSTRING", "INCLUDE", "IMPORT", "INSERT", "CHARCONST", "WCHARCONST",
  "NUM_INT", "NUM_DOUBLE", "NUM_FLOAT", "NUM_LONGDOUBLE", "NUM_UNSIGNED",
  "NUM_LONG", "NUM_ULONG", "NUM_LONGLONG", "NUM_ULONGLONG", "NUM_BOOL",
  "TYPEDEF", "TYPE_INT", "TYPE_UNSIGNED", "TYPE_SHORT", "TYPE_LONG",
  "TYPE_FLOAT", "TYPE_DOUBLE", "TYPE_CHAR", "TYPE_WCHAR", "TYPE_VOID",
  "TYPE_SIGNED", "TYPE_BOOL", "TYPE_COMPLEX", "TYPE_RAW",
  "TYPE_NON_ISO_INT8", "TYPE_NON_ISO_INT16", "TYPE_NON_ISO_INT32",
  "TYPE_NON_ISO_INT64", "LPAREN", "RPAREN", "COMMA", "SEMI", "EXTERN",
  "LBRACE", "RBRACE", "PERIOD", "ELLIPSIS", "CONST_QUAL", "VOLATILE",
  "REGISTER", "STRUCT", "UNION", "EQUAL", "SIZEOF", "ALIGNOF", "MODULE",
  "LBRACKET", "RBRACKET", "BEGINFILE", "ENDOFFILE", "CONSTANT", "RENAME",
  "NAMEWARN", "EXTEND", "PRAGMA", "FEATURE", "VARARGS", "ENUM", "CLASS",
  "TYPENAME", "PRIVATE", "PUBLIC", "PROTECTED", "COLON", "STATIC",
  "VIRTUAL", "FRIEND", "THROW", "CATCH", "EXPLICIT", "STATIC_ASSERT",
  "CONSTEXPR", "THREAD_LOCAL", "DECLTYPE", "AUTO", "NOEXCEPT", "OVERRIDE",
  "FINAL", "USING", "NAMESPACE", "NATIVE", "INLINE", "TYPEMAP", "ECHO",
  "APPLY", "CLEAR", "SWIGTEMPLATE", "FRAGMENT", "WARN", "LESSTHAN",
  "GREATERTHAN", "DELETE_KW", "DEFAULT", "LESSTHANOREQUALTO",
  "GREATERTHANOREQUALTO", "EQUALTO", "NOTEQUALTO", "LESSEQUALGREATER",
  "ARROW", "QUESTIONMARK", "TYPES", "PARMS", "NONID", "DSTAR", "DCNOT",
  "TEMPLATE", "OPERATOR", "CONVERSIONOPERATOR", "PARSETYPE", "PARSEPARM",
  "PARSEPARMS", "DOXYGENSTRING", "DOXYGENPOSTSTRING", "LOR", "LAND", "OR",
  "XOR", "AND", "LSHIFT", "RSHIFT", "PLUS", "MINUS", "STAR", "SLASH",
  "MODULO", "UMINUS", "NOT", "LNOT", "CAST", "DCOLON", "$accept",
  "program", "interface", "declaration", "swig_directive",
  "extend_directive", "$@1", "apply_directive", "clear_directive",
  "constant_directive", "echo_directive", "stringtype", "fname",
  "fragment_directive", "include_directive", "@2", "includetype",
  "inline_directive", "insert_directive", "module_directive",
  "native_directive", "pragma_directive", "pragma_arg", "pragma_lang",
  "rename_directive", "rename_namewarn", "feature_directive",
  "stringbracesemi", "featattr", "varargs_directive", "varargs_parms",
  "typemap_directive", "typemap_type", "tm_list", "tm_list_builder",
  "typemap_parm", "types_directive", "template_directive",
  "warn_directive", "c_declaration", "$@3", "c_decl", "c_decl_tail",
  "initializer", "cpp_alternate_rettype", "cpp_lambda_decl",
  "lambda_introducer", "lambda_template", "lambda_body", "lambda_tail",
  "$@4", "c_enum_key", "c_enum_inherit", "c_enum_forward_decl",
  "c_enum_decl", "c_constructor_decl", "cpp_declaration", "cpp_class_decl",
  "@5", "@6", "cpp_opt_declarators", "cpp_forward_class_decl",
  "cpp_template_decl", "$@7", "cpp_template_possible", "template_parms",
  "template_parms_builder", "templateparameter", "cpp_using_decl",
  "cpp_namespace_decl", "@8", "@9", "cpp_members", "cpp_members_builder",
  "cpp_member_no_dox", "cpp_member", "$@10", "cpp_constructor_decl",
  "cpp_destructor_decl", "cpp_conversion_operator", "cpp_catch_decl",
  "cpp_static_assert", "cpp_protection_decl", "cpp_swig_directive",
  "cpp_vend", "anonymous_bitfield", "anon_bitfield_type", "storage_class",
  "storage_class_list", "storage_class_raw", "parms", "rawparms",
  "parm_no_dox", "parm", "valparms", "valparms_builder", "valparm",
  "def_args", "parameter_declarator", "plain_declarator", "declarator",
  "notso_direct_declarator", "direct_declarator", "abstract_declarator",
  "direct_abstract_declarator", "pointer", "cv_ref_qualifier",
  "ref_qualifier", "type_qualifier", "type_qualifier_raw", "type",
  "rawtype", "type_right", "decltype", "@11", "decltypeexpr",
  "primitive_type", "primitive_type_list", "type_specifier", "definetype",
  "default_delete", "deleted_definition", "explicit_default", "ename",
  "constant_directives", "optional_ignored_defines", "enumlist",
  "enumlist_item", "edecl_with_dox", "edecl", "etype", "expr", "exprmem",
  "exprsimple", "valexpr", "exprnum", "exprcompound", "variadic_opt",
  "inherit", "raw_inherit", "$@12", "base_list", "base_specifier", "@13",
  "@14", "access_specifier", "templcpptype", "cpptype", "classkey",
  "classkeyopt", "opt_virtual", "virt_specifier_seq",
  "virt_specifier_seq_opt", "class_virt_specifier_opt",
  "exception_specification", "qualifiers_exception_specification",
  "cpp_const", "ctor_end", "ctor_initializer", "mem_initializer_list",
  "mem_initializer", "less_valparms_greater", "identifier", "idstring",
  "idstringopt", "idcolon", "idcolontail", "idtemplate",
  "idtemplatetemplate", "idcolonnt", "idcolontailnt", "string", "wstring",
  "stringbrace", "options", "kwargs", "kwargs_builder", "stringnum", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-969)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-607)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     687,  7563,  7664,   238,   113,  7009,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,   285,    31,    31,    59,  -969,  -969,   -23,   107,  8378,
     282,  -969,   139,  8588,  -969,   355,   850,  -969,  -969,  -969,
    3150,  -969,   282,   107,  -969,   -25,  -969,   201,  -969,  8070,
    -969,   108,  -969,  -969,  -969,   191,  -969,  -969,    54,   206,
    8236,  -969,  -969,   206,   214,   236,   258,  -969,  -969,  -969,
     301,  -969,   345,  -969,  -969,   292,   409,   352,   187,   358,
     212,  8448,  8448,   367,   422,   467,   439,  8658,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,   206,
    -969,  -969,  -969,  -969,  -969,  -969,  6500,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  8518,   898,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,   255,  2662,  -969,  -969,
    -969,  -969,  -969,   850,  6670,  -969,  2527,  3202,    62,  1439,
    1507,   850,   282,  -969,  -969,   431,   377,   431,   430,   507,
     403,   850,  -969,  -969,  -969,    92,   514,  -969,  -969,   490,
    -969,    45,  -969,   357,  -969,  8850,    79,   357,   357,    74,
    6502,   485,   135,   434,    92,    92,   357,  7868,  8070,  -969,
     282,   282,   248,  -969,   168,   521,    92,  -969,  -969,   357,
    -969,   566,  8070,   547,  1184,   564,   584,  -969,   357,   467,
     566,  8070,  -969,  -969,  -969,   282,  7969,   282,   282,  -969,
    -969,   282,   453,   467,   235,   163,  1745,   357,   443,  1646,
     502,  6502,   282,  6536,   333,   467,  -969,  2279,    92,  -969,
    -969,   -25,    23,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  6172,  9231,   597,   609,
    6172,  6172,  6172,  6172,  6172,  6172,  -969,   569,   638,  -969,
     643,  1141,  3439,    75,  -969,    89,  -969,  -969,   685,   698,
     850,  6783,  1880,  1880,   645,   655,  1652,   585,  -969,  -969,
    -969,   649,  -969,  -969,  -969,  3439,  -969,  -969,  6302,   671,
    6783,   575,   282,   452,   430,  -969,  -969,   575,   452,   430,
    -969,   850,  -969,  -969,  8070,  3337,  -969,  8070,  3472,   575,
    2015,  2150,   850,   452,   430,  -969,   604,  1639,  -969,  -969,
     -25,   724,  7765,  -969,   725,  -969,  -969,   690,   566,   282,
     282,  -969,  -969,   716,   744,   750,  -969,  -969,  -969,   216,
     431,   455,  2797,  -969,  -969,  -969,  -969,   282,   754,   759,
     775,   777,   790,  -969,   794,   801,   806,  -969,  8588,  -969,
     282,  -969,   798,   810,  -969,   817,  8448,  -969,  -969,   468,
    -969,  -969,  8448,  -969,   821,  -969,   465,   306,   827,   475,
     741,   769,   831,  -969,  -969,   272,   841,  -969,  -969,  -969,
    -969,    60,  1080,  1080,   218,   543,   774,   851,   146,   575,
     575,   848,  8070,   575,  2533,  2668,   779,    87,  1020,    17,
     829,   474,  6896,  1020,  -969,   857,  -969,   610,   566,   868,
    -969,  2246,   107,  -969,   908,   911,  6642,   813,    23,  -969,
     883,  -969,  -969,  -969,  1984,  -969,  -969,  -969,  -969,  -969,
    -969,  2662,  -969,  3607,  3742,  3877,  4012,  6172,  6172,  4147,
    4282,  4417,  4552,  4687,  4822,  4957,  5092,  5227,  5362,  5497,
    5632,   922,   924,  -969,  -969,  -969,   410,   410,  6628,   816,
     481,  -969,   495,  -969,  -969,   410,   410,   501,   820,  1500,
    -969,  -969,  8070,  6448,    25,   452,  -969,  8070,  5767,   452,
     896,  -969,  9167,   897,  -969,  9199,   452,   575,   452,   430,
    -969,   575,   452,   430,  -969,   850,  1595,   575,   906,  2803,
     452,   850,  -969,  -969,  8070,  -969,  8070,   343,   905,  -969,
    -969,  1418,  -969,   357,   914,  8070,   919,   918,  -969,   280,
    2753,   927,  8070,  6502,   920,  -969,  -969,  1184,  7122,   926,
    -969,   898,  8448,   932,   931,  8070,  -969,   670,   930,   357,
    8070,    80,  -969,  -969,  7969,   888,  7969,  -969,  8070,  -969,
    1500,  1207,   472,    64,  -969,   942,   575,   575,  1714,   948,
     326,   913,   907,  -969,  -969,   799,  -969,   601,  -969,  -969,
    -969,    36,  1874,  8920,   396,   962,   774,    38,   934,  -969,
    -969,  -969,   933,  -969,  -969,  -969,   967,   974,  6442,  5902,
    2932,  3067,  6307,   268,  6037,    92,  -969,  -969,  1196,  -969,
    1196,  -969,  1736,  -969,  1736,  1664,  2904,  -969,  3709,  -969,
    3844,  -969,  3979,  -969,  2254,  -969,  1984,  -969,   858,  -969,
     858,  -969,   782,  -969,   782,  -969,  -969,  -969,  -969,  -969,
    -969,   978,   979,   820,   850,  -969,  -969,  -969,   488,   983,
     984,  1500,   985,   629,   820,  -969,   426,   990,  -969,  9232,
    1020,  -969,   601,  -969,   452,   452,   575,   972,  2938,   452,
     430,   452,   575,   575,   452,  -969,  -969,  -969,  -969,   566,
    8070,  7235,  -969,  -969,   306,  -969,   994,  -969,   991,  -969,
    -969,  -969,  -969,   566,   996,    83,   995,  1498,  -969,  1020,
    -969,  1008,  -969,  -969,  7348,  8588,   809,  8070,  -969,   997,
     915,  -969,   176,   951,  -969,  1017,  1016,  -969,  -969,  -969,
    1023,  -969,   566,  -969,   964,   282,   598,  -969,  1026,  -969,
    1500,  1020,   183,   575,  -969,  8070,  6172,  -969,  -969,  -969,
    -969,  -969,  -969,  8990,  1025,  -969,   973,  1034,  -969,  -969,
    1018,   796,  2009,  6172,   787,  -969,  -969,  1036,  -969,   878,
    2398,  1038,  -969,  -969,  -969,  1043,  3034,  6172,  6172,  8588,
    6172,  1045,  1047,  1049,  3169,  1051,  6172,  -969,  -969,  -969,
    -969,  1052,  1053,  -969,  -969,   654,  -969,  -969,  -969,  -969,
     452,   575,   575,   452,   452,   452,  1054,  -969,  1418,   601,
    2398,  1418,   357,  1055,  -969,  1020,  1061,  -969,  -969,  6502,
     144,  -969,  8448,  -969,  1056,   601,    84,    92,   153,  -969,
    2662,   276,  -969,  8070,   577,  1062,    54,  8754,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  8306,  -969,  7461,  1063,  -969,
    1069,  3304,   691,  -969,  -969,   397,  -969,  1018,  -969,    61,
    1075,   124,  8070,  2797,  1072,  1040,  -969,  1091,  3439,  -969,
    6502,  -969,  -969,  -969,   934,  -969,  -969,  -969,   282,  -969,
    -969,  -969,  1090,  1064,  1066,  1067,  9039,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  1092,  8135,  1012,
    -969,  -969,  -969,  -969,  -969,  -969,  -969,  -969,  1376,  -969,
    -969,  -969,  -969,  -969,  -969,  6172,  6172,  -969,  -969,  -969,
    3574,  -969,  -969,  -969,   452,   452,  -969,  -969,  -969,   431,
    1098,   566,   995,  1254,    80,  -969,  1102,  -969,  1104,  -969,
    -969,  -969,   176,  -969,  -969,   176,  1046,  -969,  -969,  1109,
     500,  8588,  7969,  6502,  8588,  6553,  -969,  -969,   691,  -969,
    -969,   351,  8070,  -969,   573,  -969,    92,  1018,  -969,  1097,
    6481,    15,  -969,  1111,  1113,  1112,   721,   282,   513,  -969,
    1020,  -969,    31,   934,  -969,  -969,  -969,  -969,  -969,  6483,
    9123,  -969,  -969,  1088,  1089,  1095,    92,  1096,  1101,   164,
    2398,  -969,  -969,  1122,  -969,  -969,  -969,  1020,  -969,  -969,
    -969,  -969,  -969,  1129,  1134,  -969,   431,  1184,  1076,   150,
     253,  6896,  -969,   787,  1138,  -969,  -969,  -969,  -969,  -969,
    6172,  -969,  1020,  1018,  -969,  1018,   693,  -969,  1137,  1140,
    1136,   454,  -969,  -969,   351,  -969,   282,  2398,  -969,  -969,
    1020,  1147,  6172,  -969,  1145,  -969,  1149,  -969,  -969,  -969,
    1153,  -969,  2144,  8070,  1150,  1160,    28,  1161,  -969,  1020,
    1158,  -969,  3439,   351,  -969,  -969,  -969,  -969,  -969,   282,
    -969,  -969,  -969,   787,    31,  1157,   351,  8070,  6751,  6483,
    -969,  -969,  1164,  8070,  8070,  8070,  1166,   796,    50,  -969,
     787,  1159,  -969,  -969,  -969,   787,  1169,  -969,  -969,  1020,
    1176,  1182,  1192,  8070,  -969,  8588,   573,  -969,  -969,  -969,
    1020,  -969,   689,  1020,  1020,  1020,  1194,  1180,  -969,  -969,
    -969,  -969,  2797,  -969,  -969,  -969,  1020,   573,  1193,  -969,
    -969,  -969
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     0,     0,     4,   587,   402,   410,
     403,   404,   407,   408,   405,   406,   389,   409,   388,   411,
     391,   412,   413,   414,   415,   281,   378,   379,   380,   544,
     545,   147,   527,   527,     0,   588,   589,     0,   599,     0,
       0,   282,     0,     0,   376,   293,   383,   394,   387,   399,
     400,   543,     0,   606,   392,   597,     6,     0,     8,   279,
       1,    17,    62,    58,    59,     0,   266,    16,   263,   630,
       0,    78,    79,   630,    74,     0,     0,   265,   267,   268,
       0,   269,     0,   270,   271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,    11,
       9,    13,    20,    21,    22,    23,    24,    25,    26,   630,
      27,    28,    29,    30,    31,    32,     0,    33,    34,    35,
      36,    37,    38,    14,   105,   110,   107,   106,    18,    15,
     156,   157,   158,   159,   160,   161,   113,     0,   259,   261,
     149,   148,   526,   541,   542,   395,     0,   285,   600,   283,
     390,     3,   377,   382,   279,   386,     0,     0,   599,   527,
     527,   370,     0,   296,   280,   293,   305,   293,   350,   527,
     331,   384,   401,   393,   607,     0,     0,   595,     5,     0,
     272,   273,    19,     0,   621,     0,   264,     0,     0,     0,
       0,   381,   606,   550,     0,     0,     0,   279,   279,   236,
       0,     0,     0,   194,   606,     0,     0,    60,    61,     0,
      48,    49,   279,     0,   301,     0,    96,    97,   594,     0,
     104,   279,   128,   127,   130,     0,   182,     0,     0,   136,
     126,     0,   131,     0,     0,     0,     0,     0,   305,     0,
     331,     0,   423,     0,   531,   263,   262,     0,     0,   601,
     608,   598,   587,   625,   455,   456,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   288,     0,   284,   286,
     442,   381,     0,   452,   462,   441,   451,   463,   453,   454,
     385,   279,   527,   527,     0,     0,   527,   392,   292,   420,
     421,   290,   417,   418,   419,   416,   441,   361,     0,     0,
     279,   526,     0,   307,   352,   354,   323,   526,   306,   351,
     353,   369,   332,   294,   279,     0,   295,   279,     0,   526,
     527,   527,   368,   302,   345,   344,   323,   355,   605,   604,
     603,     0,   279,   275,   274,   591,   590,     0,   592,     0,
       0,   620,   108,   634,     0,   631,    66,    47,    46,     0,
     293,   305,     0,   547,   548,   546,   549,     0,     0,    70,
      88,     0,     0,    90,     0,     0,     0,   189,     0,    12,
       0,   192,     0,     0,    95,     0,     0,    99,   298,   305,
     299,    42,     0,   593,     0,    52,     0,    51,     0,     0,
       0,     0,   181,   183,   188,   543,     0,   129,   171,   133,
     132,     0,     0,     0,     0,   590,     0,     0,     0,     0,
       0,     0,   279,     0,     0,     0,   323,     0,   571,   331,
     151,   606,   279,   571,   529,     0,   528,   393,   264,     0,
     396,     0,     0,   596,     0,     0,     0,   462,     0,   457,
       0,   461,   458,   459,   471,   522,   521,   472,   523,   524,
     586,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   624,   623,   622,   527,   527,   527,   392,
       0,   331,     0,   366,   363,   527,   527,     0,   331,   355,
     291,   362,   279,   527,   392,   316,   324,   279,     0,   315,
       0,   340,     0,     0,   359,     0,   312,   526,   304,   349,
     347,   526,   303,   348,   346,   367,   357,     0,     0,     0,
     308,   356,   602,     7,   279,   276,   279,     0,     0,   173,
      12,     0,   629,     0,     0,   279,     0,     0,    73,     0,
       0,     0,     0,     0,     0,   190,   191,   301,     0,     0,
      12,   260,     0,   100,     0,   279,    98,     0,     0,     0,
       0,     0,   135,   134,   182,   169,     0,   185,   279,    56,
       0,     0,     0,     0,    75,     0,     0,     0,     0,     0,
     562,   553,   554,   375,   374,   567,   373,   371,   563,   568,
     570,     0,     0,     0,     0,     0,   392,   293,   552,   164,
     168,   559,   531,   398,   397,   609,   447,   443,   464,     0,
       0,     0,   370,     0,     0,     0,   287,   518,   500,   517,
     499,   513,   495,   514,   496,   519,     0,   512,   494,   511,
     493,   507,   489,   508,   490,   506,   488,   509,   491,   510,
     492,   501,   483,   502,   484,   503,   485,   504,   486,   505,
     487,   449,   445,     0,   355,   335,   334,   333,   357,     0,
       0,   356,     0,     0,   323,   325,   355,     0,   328,     0,
     342,   341,   364,   360,   314,   313,     0,     0,     0,   309,
     358,   317,     0,     0,   311,   278,   277,    64,    65,    63,
     279,     0,   636,   639,   638,   632,   635,    44,     0,    43,
      39,    72,    69,    71,     0,   638,    88,     0,    91,   571,
     235,     0,   195,   196,     0,     0,     0,   279,    41,     0,
       0,   614,   612,     0,    55,     0,     0,   628,    85,   627,
       0,   102,   626,    84,     0,     0,   260,   184,     0,    12,
       0,   571,     0,     0,   342,   279,     0,   565,   556,   555,
     569,   372,   117,     0,     0,   140,   142,     0,   150,   152,
     427,   571,     0,     0,     0,   124,   551,   530,   532,   534,
       0,     0,   448,   444,   465,     0,     0,     0,     0,   369,
       0,     0,     0,     0,     0,     0,     0,   450,   446,   336,
     338,     0,     0,   343,   326,     0,   330,   329,   297,   365,
     318,     0,     0,   310,   322,   321,     0,   109,     0,   342,
       0,     0,     0,     0,    82,   571,     0,   111,   193,     0,
     606,    93,     0,    92,     0,   342,     0,     0,     0,   610,
     285,     0,    50,   279,     0,     0,   263,     0,   175,   176,
     179,   178,   170,   177,   180,     0,   172,     0,     0,    77,
       0,     0,   558,   119,   141,     0,   118,   424,   426,   434,
       0,   428,   279,     0,   568,   579,   155,     0,   125,   123,
       0,   120,   122,   114,   552,   539,   538,   540,     0,   536,
     201,   222,     0,     0,     0,     0,   260,   247,   248,   241,
     249,   220,   216,   245,   240,   242,   243,   244,   246,   221,
     217,   218,   204,   211,   210,   214,   213,     0,   260,   223,
     202,   205,   206,   209,   215,   207,   208,   219,     0,   162,
     515,   497,   468,   467,   466,     0,     0,   516,   498,   460,
     520,   337,   339,   327,   320,   319,   174,   637,   633,   293,
       0,   638,    88,     0,     0,    89,     0,    67,     0,   101,
     300,   615,   613,   619,   618,   617,     0,    53,    54,     0,
     293,     0,   182,     0,     0,     0,    57,    76,   561,   566,
     557,   293,   279,   143,     0,   425,     0,   427,   436,   438,
       0,   427,   430,   429,     0,     0,     0,     0,     0,   115,
     571,   533,   527,   552,   226,   238,   237,   239,   224,     0,
     260,   203,   225,   389,   388,   391,     0,     0,   387,   392,
       0,   470,   469,     0,    40,    87,    83,   571,    80,    68,
      94,   611,   616,     0,     0,   187,   293,   301,     0,   331,
       0,   279,   564,     0,     0,   145,   144,   139,   437,   435,
       0,   153,   571,   427,   431,   427,     0,   576,     0,   578,
     580,     0,   572,   573,   293,   535,     0,     0,   166,   165,
     571,     0,     0,   212,     0,    45,     0,   103,    86,   186,
       0,   169,     0,   279,     0,     0,     0,     0,   116,   571,
       0,   439,   440,   293,   433,   432,   574,   575,   577,     0,
     582,   584,   585,     0,   527,     0,   293,   279,     0,     0,
      81,   112,     0,   279,   279,   279,     0,   571,     0,   146,
       0,   581,   121,   537,   227,     0,     0,   253,   163,   571,
       0,     0,     0,   279,   228,     0,     0,   154,   583,   167,
     571,   234,     0,   571,   571,   571,     0,     0,   137,   229,
     250,   252,     0,   232,   231,   230,   571,     0,     0,   233,
     138,   251
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -969,  -969,  -347,  -969,  -969,  -969,  -969,     5,    26,    37,
      40,  -497,  -969,    41,    42,  -969,  -969,  -969,    51,  -969,
    -969,    52,  -969,  -969,    71,  -969,    78,  -667,  -664,    97,
    -969,    99,  -969,  -333,  -969,   -63,   100,   103,   104,   105,
    -969,   492,  -968,  -922,  -177,  -969,  -969,  -969,  -923,  -704,
    -969,   -74,  -969,  -969,  -969,  -969,  -969,    16,  -969,  -969,
     130,    21,    24,  -969,  -969,  -552,  -969,   664,   110,  -969,
    -969,  -969,  -769,  -969,  -835,   323,  -969,   499,  -969,   503,
     111,  -969,  -969,  -969,   -98,  -969,  -969,    10,  -969,  1108,
     -53,  -969,  1208,    18,   413,  -969,   789,    -5,  -969,  -540,
      32,  1144,   -36,   -31,   -62,   -27,  -620,   657,   -41,    27,
     -58,    -1,    -2,   -84,  -969,  -969,   -48,  1205,  -969,  -335,
    -152,  -969,  -969,  -969,   391,   274,  -687,  -969,  -969,   278,
    -969,   887,  -969,   590,  -131,  -502,  -969,   -30,   650,  -969,
    -969,  -969,   381,  -969,  -969,  -969,  -215,    -6,  -969,  -969,
     266,  -565,  -969,  -969,  -571,   592,  -334,   158,  -969,  -969,
     178,    -3,   980,  -115,  -969,   706,  -197,  -139,  1103,  -969,
    -337,  1033,  -969,   555,   321,  -179,  -969,  -507
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     4,     5,   100,   101,   102,   820,   897,   898,   899,
     900,   395,   396,   901,   902,   749,   109,   110,   903,   112,
     113,   904,   712,   195,   905,   116,   906,   741,   551,   907,
     372,   908,   383,   215,   216,   217,   909,   910,   911,   912,
     540,   124,   883,   774,   227,   125,   766,   865,   984,  1047,
    1090,    40,   604,   126,   127,   128,   129,   913,  1020,   780,
    1069,   914,   915,   746,   852,   401,   402,   403,   916,   134,
     560,   379,   917,   918,   919,   920,  1067,   921,   922,   923,
     924,   136,   925,   926,  1141,   927,  1017,   928,   138,   139,
     294,   180,    41,   181,   277,   278,   279,   775,   164,   387,
     388,   313,   238,   295,   168,   239,   595,   596,    43,    44,
     280,   191,    46,    47,   247,   440,    48,    49,    50,   301,
     345,   303,   304,   430,   868,   869,   870,   871,   987,   988,
    1091,   305,   283,   284,   306,   286,   287,   335,   435,   436,
     608,   777,   778,   888,  1003,   889,    51,    52,   366,   367,
     779,   598,   981,   612,   599,   600,  1142,   876,   998,  1059,
    1060,   174,    53,   353,   394,    54,   177,    55,   251,   733,
     839,   288,   289,   743,   188,   354,   355,   705
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,    45,   143,   144,   302,   171,   179,   250,   349,   166,
     103,   405,   190,   229,   167,   137,   285,   721,   169,    42,
      57,   130,   744,   228,   760,   757,   131,   546,   213,   132,
     384,   104,   558,   214,   214,   148,   338,   250,    45,   703,
     163,   153,   105,   716,   702,   106,   107,   108,   703,   230,
     824,   950,   823,   564,   443,   417,   111,   114,    45,  1043,
     184,  1008,   809,   242,     7,   309,   351,   675,   347,  1115,
     152,   444,   602,   356,   357,  1088,   115,   165,    70,   243,
     142,   370,   762,   117,   737,   351,   184,     7,   342,   351,
       7,   231,   175,   772,   601,     7,   983,   314,   319,   607,
     145,   229,   118,   393,   119,   120,   675,   334,   121,   122,
     123,   228,   290,    60,   773,   133,   135,   176,   358,   146,
     321,   579,   421,   481,   738,   352,   739,   296,   418,   315,
     320,   244,  -289,   427,   445,  1122,   527,   230,  -592,   151,
    1053,   676,  1103,   532,   373,   374,   281,   763,   237,     7,
      35,    36,  1137,    45,   361,   148,     7,  1139,  1116,   385,
     323,  1135,   326,   332,   147,   276,     7,   991,   398,   343,
     344,  1120,   185,    35,    36,  1008,    35,    36,   389,   350,
     753,    35,    36,   390,  1125,   986,   482,   169,   957,   351,
     362,   207,  -289,   701,   740,  1136,    45,    45,   152,   949,
     874,   178,    37,   961,   418,  1082,    38,   414,  1073,   147,
     302,    45,   419,   724,   528,   960,   210,   529,   184,     7,
      45,     7,   360,   380,  1157,    45,   312,   859,   182,   726,
     314,   319,   183,   208,   334,    35,    36,   147,     7,    58,
    -258,   184,    35,    36,   404,   166,   147,   187,   992,   993,
     167,  1074,    35,    36,   169,   194,   490,   492,     7,   359,
     497,    37,   315,   320,   488,    38,   427,   767,   519,   523,
     147,   510,   964,   428,   513,   433,   163,   196,    37,    59,
     967,   161,    38,   503,   711,     7,   184,  1028,  1025,   535,
      45,   525,   377,   837,  1083,     7,   531,   980,  1105,   197,
     520,   524,   312,   378,  1054,    35,    36,    35,    36,    45,
     790,   948,   351,   165,   952,  -293,   703,   152,   838,   703,
     557,   947,   968,    45,    35,    36,    45,   156,   214,   566,
     285,    37,   414,    37,   214,    38,     7,   158,   140,   299,
     300,    45,   198,   412,    35,    36,   413,   697,   152,   184,
      37,   161,   570,   332,    38,   544,   141,   162,     7,   405,
       7,   405,   412,   184,   200,   413,  1094,   756,  1095,   585,
     161,    35,    36,   248,   249,  -293,   490,   492,   497,   605,
    1084,    35,    36,  1085,   201,   826,   199,   597,   161,   698,
     735,   582,   597,   206,   193,   791,   154,    37,   792,   209,
     577,    38,   857,   332,   155,   414,   156,    37,   218,   434,
     156,    38,     7,  1042,   157,   591,   592,   858,   324,   623,
    1038,    45,    35,    36,   314,   319,   334,   773,   706,     7,
     233,    45,  1148,   519,   523,   714,   325,   875,   982,   615,
     769,   334,   770,   983,    35,    36,    35,    36,    37,   672,
     302,   291,    38,  1160,   677,   203,   315,   320,   671,   142,
     281,   299,   300,   219,   690,   520,   524,   418,   670,   157,
      37,   327,   169,   184,   158,     7,    26,    27,    28,   276,
     221,   695,   159,   696,   422,   160,   156,   363,   364,   328,
     161,   954,   708,   507,   162,  1101,   545,  1080,    35,    36,
    1102,    45,   325,     7,   214,   365,    45,   568,   569,   565,
       7,   508,   729,   418,   325,    35,    36,     7,  -422,   337,
    -422,   389,   422,   665,    37,   748,   390,   325,    38,   291,
     169,   669,   341,    45,   155,    45,   422,   666,   995,   750,
     325,    37,   422,   667,    45,    38,   874,   157,   310,   670,
    -422,    45,   152,   169,   325,   156,   329,  1062,   152,  1063,
     325,    35,    36,   103,    45,   312,   157,   381,   137,    45,
     718,   725,   351,    45,   130,    45,   147,    45,     7,   131,
     409,   789,   132,   410,   104,   719,   862,    37,   736,    35,
      36,    38,   404,   386,   404,   105,    35,    36,   106,   107,
     108,   768,   572,    35,    36,   573,   690,   332,   391,   111,
     114,   312,   669,   751,  1045,    37,   418,  1046,   427,    38,
     302,    66,    37,   531,   152,  1031,    38,   392,  1032,   115,
      29,    30,   248,   339,   330,   531,   117,   331,   452,   597,
     802,   597,   332,   846,   169,   147,   312,   816,    32,    33,
     453,    26,    27,    28,   610,   118,  -560,   119,   120,  -606,
    -606,   121,   122,   123,    35,    36,  1064,   829,   133,   135,
     507,   804,   460,     7,   834,    77,    78,    79,   597,   229,
      81,   461,    83,    84,   462,  -606,  -560,   493,   508,   228,
      37,   351,   483,  1076,    38,   507,   943,   494,   152,    45,
     611,   499,   860,   801,   484,   485,   103,   714,   500,   285,
     597,   137,   502,   508,   312,   230,   847,   130,  1093,   802,
     526,   302,   131,   169,   533,   132,    45,   104,   593,   103,
     597,   594,   537,  1150,   137,  1151,  1106,  1096,   105,  1097,
     130,   106,   107,   108,  1152,   131,   150,   767,   132,   825,
     104,   170,   111,   114,    45,  1118,   855,   405,   173,    35,
      36,   105,   849,   229,   106,   107,   108,   850,   536,   958,
     851,   541,   115,   228,   214,   111,   114,   152,   597,   117,
     591,   592,   801,   875,   597,   730,   542,   153,   879,   731,
     969,   202,   205,   543,   597,   115,   548,   975,   118,   230,
     119,   120,   117,   232,   121,   122,   123,   867,     1,     2,
       3,   133,   135,   737,   549,   184,   152,  1158,   550,   994,
     552,   118,   240,   119,   120,   299,   300,   121,   122,   123,
     880,   881,   553,   882,   133,   135,   554,   872,   970,   281,
     561,   231,    45,   574,  1058,   555,    26,    27,    28,   244,
     556,   873,   562,   831,   242,   739,   447,   451,   276,   563,
     297,   956,   103,   567,   832,   316,   316,   137,   322,   571,
     975,    45,   575,   130,   576,   336,   589,  1071,   131,   589,
    1018,   132,   578,   104,   590,   591,   592,   590,   591,   592,
     580,   232,   584,   581,   105,   588,   240,   106,   107,   108,
      26,    27,    28,   609,   867,   603,   375,   376,   111,   114,
     613,   616,  1000,  1037,   617,   624,  1040,   478,   479,   480,
     170,    66,   244,   593,   625,   661,   594,   662,   115,  1044,
     302,   399,   664,   406,   407,   117,   668,   408,   680,   682,
     416,   316,   316,   245,  1023,   426,   700,   429,   150,   240,
     437,   885,   886,   887,   118,   692,   119,   120,   707,   597,
     121,   122,   123,   709,   710,  1035,   720,   133,   135,   717,
     723,    45,  1065,   727,   734,    77,    78,    79,   728,   745,
      81,    45,    83,    84,   754,  1027,   597,   170,  1087,   755,
     404,   476,   477,   478,   479,   480,   759,   489,   491,   491,
     302,   389,   498,   758,   771,   428,   390,   433,   782,   434,
     169,   597,   776,  1086,   414,   783,   504,   316,   506,   797,
     798,   811,  1052,   316,   867,   799,   800,   803,   867,   597,
    1112,  1079,   806,   819,   282,   316,   316,   316,   822,   835,
      45,  1070,  1149,   316,   308,  1153,  1154,  1155,   597,   818,
     192,   821,   827,   840,  1126,   538,   539,   836,  1159,   841,
    1130,  1131,  1132,   842,   843,   416,   204,   844,   856,   863,
      26,    27,    28,   547,  1123,   864,   597,  1147,   866,   884,
    1146,    70,    45,     7,   929,   930,   559,   935,   597,   936,
     867,   937,   867,   939,   941,   942,   946,   953,   959,   597,
     589,   186,   597,   597,   597,   955,    45,   977,   590,   591,
     592,   978,    45,    45,    45,   597,   997,   971,   491,   491,
     491,   359,   990,   211,   583,   316,   316,   996,   220,   316,
     316,   316,    45,   316,   441,   999,  1004,  1012,   606,  1009,
    1005,  1070,  1006,  1007,     7,  1024,  1029,   593,  1030,  1033,
     594,  1034,  1050,   446,  1055,  1056,  1057,   454,   455,   456,
     457,   458,   459,   346,  -256,  -255,  1075,   346,   346,    35,
      36,  -257,  1072,  1077,   368,   369,   346,  -254,  1078,  1081,
    1089,  1098,   154,  1099,  -293,  1100,   382,     7,  1107,   346,
     155,  1113,  1109,  1110,   663,    37,   156,  1111,   346,   158,
     157,  1114,  1119,  1117,  1124,   170,  1129,  1133,  1138,   674,
       7,  1140,   512,   184,   415,   515,   348,   346,  1143,   162,
     348,   348,   431,   316,  1144,   154,   983,   316,   442,   348,
      35,    36,   316,   316,  1145,   316,  1156,  1161,   848,  1128,
     747,  1011,   348,   157,  -293,   853,   246,   149,   359,   854,
     626,   348,   397,   966,   761,   172,    37,     7,   985,   240,
     158,  1049,   781,   170,  1048,  1001,   411,   348,   159,  1066,
     348,   160,   808,    35,    36,  1134,   161,  1121,   438,   340,
     162,   833,     0,     0,     0,     0,   170,   240,   316,     0,
       0,     0,   316,   316,   316,   359,    35,    36,  1026,    37,
       0,     0,     0,   158,   318,     0,   467,     0,     0,     0,
       0,   159,     0,   333,   160,     0,     0,     0,     0,   161,
       0,     0,    37,   162,     0,     0,   158,   474,   475,   476,
     477,   478,   479,   480,   235,     0,     0,   236,     0,     0,
       0,     0,   161,    35,    36,     0,   162,     0,   282,     0,
     628,   630,   632,   634,   635,   636,   638,   640,   642,   644,
     646,   648,   650,   652,   654,   656,   658,   660,     0,    37,
       0,     0,     0,   158,     0,     0,     0,   170,     0,     7,
     318,   235,   316,   333,   236,     0,     0,     0,     0,   161,
       0,     0,   316,   162,   316,   679,     0,     0,   316,   316,
       8,     9,    10,    11,    12,    13,    14,    15,  1013,    17,
    1014,    19,  1015,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,   240,   184,     0,    26,    27,    28,    29,
      30,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,     0,     7,     0,     0,     0,    31,    32,    33,     0,
       0,   845,     0,     0,     0,   505,   170,     0,     0,   316,
       0,   509,    34,   241,     0,    35,    36,     0,     0,   232,
       0,     0,     0,   516,   518,   522,     0,     0,     0,     0,
     310,   530,     0,     0,     0,     0,     0,     0,   311,     0,
       0,    37,     0,     0,     0,    38,   974,     0,   157,     0,
       0,     7,   737,     7,   184,   784,   786,   640,   646,   656,
       7,   794,     0,     0,     0,  1016,     0,   316,   316,     0,
       0,     0,     0,   346,     0,     0,     0,     0,    35,    36,
     346,     0,     0,     0,     0,   240,     0,     0,     0,   359,
       0,   154,   738,     0,   739,     0,     0,   732,   310,   346,
      26,    27,    28,   232,    37,     0,   317,     0,    38,   157,
       0,     0,     0,   505,   509,     0,   157,   516,   518,   522,
     699,   530,     0,     0,   704,     0,   348,     0,   312,     0,
       0,     0,   713,   715,     0,     0,   240,    35,    36,    35,
      36,     0,     0,     0,  1002,     0,    35,    36,     7,     0,
       0,     0,   348,     0,   742,   795,     0,     0,     0,     0,
       0,     0,   740,    37,   752,    37,     0,   158,     0,   158,
       0,     0,    37,     0,     0,   235,    38,   159,   236,     0,
     160,     0,     0,   161,  1019,   161,   310,   162,     0,   162,
       0,     0,     7,   861,   686,     0,   312,   673,     0,     7,
       0,     0,     0,     0,   157,     7,     0,     0,     0,   240,
     878,   684,     0,     0,     0,   685,     0,     0,     0,     0,
     689,   691,     0,   694,   932,   933,  1036,   934,     0,  1039,
     418,   240,     0,   940,    35,    36,     0,   418,   527,    26,
      27,    28,     0,   154,     0,   423,   240,     0,     0,     0,
       0,   142,     0,  1061,     0,   830,     0,     0,     0,     0,
      37,   157,     0,     0,    38,   240,     0,     7,     0,     0,
       0,     0,   687,     0,     0,   688,   673,   282,    35,    36,
     684,   685,   689,     0,   312,    35,    36,     0,     0,     0,
       0,    35,    36,   170,     0,     0,     0,   606,     7,     0,
     742,     0,     0,     0,    37,   418,     0,     0,    38,   742,
       0,    37,     0,   686,     0,    38,   528,    37,     0,   529,
       0,   158,  1104,   424,     0,     0,   425,     0,   312,   495,
       0,   332,   496,     0,     0,   312,   418,   332,     0,     0,
       0,   162,     0,     0,   420,   474,   475,   476,   477,   478,
     479,   480,   346,    35,    36,  1061,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   240,   962,   963,   965,     0,
     805,     0,  1021,  1022,     0,     0,     0,     0,     0,    37,
     810,     0,   813,    38,    35,    36,   814,   815,     0,     0,
       0,   687,   463,   464,   688,     0,   467,     0,     0,   989,
       0,   704,     0,   312,   951,   348,     0,     0,     0,     0,
      37,     0,     0,     0,    38,     0,     0,   474,   475,   476,
     477,   478,   479,   480,     0,   764,     0,   252,     0,   438,
     184,   253,     0,     7,   312,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   805,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,   266,     0,     0,     0,     0,
       0,   154,     0,     0,    26,    27,    28,    29,    30,   142,
     267,   268,     0,   765,     0,     0,     0,  1092,     0,   157,
       0,     0,     0,     0,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,   944,   945,     0,     0,  1108,
      34,     0,   269,    35,    36,     0,   989,     0,     0,    35,
      36,     0,     0,     0,     0,     0,     0,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,   742,     0,    37,
       0,     0,     0,    38,     0,    37,     0,     0,     0,   158,
       0,     0,     0,     0,   270,     0,     0,   271,   272,   273,
     877,     0,   252,   274,   275,   184,   253,     0,     7,   162,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
     266,     0,     0,     0,     0,   298,   310,     0,     0,    26,
      27,    28,    29,    30,   517,   267,   268,     0,     0,     0,
       0,     0,     0,     0,   157,     0,     0,     0,     0,    31,
      32,    33,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,   465,   466,   467,    34,     0,   269,    35,    36,
       0,     0,     0,     0,    35,    36,     0,     0,     0,     0,
       0,     0,     0,   299,   300,   474,   475,   476,   477,   478,
     479,   480,     0,     0,    37,     0,     0,     0,    38,     0,
      37,     0,     0,     0,    38,     0,     0,     0,     0,   270,
       0,     0,   271,   272,   273,   764,     0,   252,   274,   275,
     184,   253,     0,     7,   312,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,   266,     0,     0,     0,     0,
       0,   310,     0,     0,    26,    27,    28,    29,    30,   521,
     267,   268,     0,     0,     0,     0,     0,     0,     0,   157,
       0,     0,     0,     0,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,   269,    35,    36,     0,     0,     0,     0,    35,
      36,     0,     0,     0,     0,     0,     0,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,    38,     0,    37,     0,     0,     0,    38,
       0,     0,     0,     0,   270,     0,     0,   271,   272,   273,
     439,     0,   252,   274,   275,   184,   253,     0,   614,   312,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
     266,     0,     0,     0,     0,     0,     0,     0,     0,    26,
      27,    28,    29,    30,     0,   267,   268,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,   463,   464,   465,   466,   467,     0,   468,     0,
     463,   464,   465,   466,   467,    34,     0,   269,    35,    36,
       0,     0,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   473,   474,   475,   476,   477,   478,
     479,   480,     0,     0,    37,     0,     0,     0,    38,   890,
       0,  -260,    62,     0,     0,     0,    63,    64,    65,   270,
       0,     0,   271,   272,   273,     0,     0,     0,   274,   275,
       0,    66,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,     0,
       0,     0,   891,    68,     0,  -199,     0,     0,  -260,  -260,
    -260,  -260,  -260,     0,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,   892,    74,    75,    76,  -260,  -260,
    -260,   893,   894,   895,     0,    77,    78,    79,     0,    80,
      81,    82,    83,    84,  -260,  -260,     0,  -260,  -260,    85,
       0,     0,     0,    89,    90,    91,    92,    93,    94,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,  -260,     0,     0,    97,  -260,  -260,     0,
       0,     0,   896,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,   184,   253,     0,     7,  -260,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,   298,   418,     0,     0,    26,    27,    28,
      29,    30,   586,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,    35,    36,     0,     0,     0,     0,     0,     0,
       0,   299,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,    37,     0,
       0,     0,    38,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     7,   312,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,   418,
       0,    25,    26,    27,    28,    29,    30,   587,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     7,    35,    36,   184,
       0,     0,     0,     0,     0,     0,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,     0,    37,     0,     0,
       0,    38,     0,    37,     0,     0,    39,    38,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     7,   312,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,    35,    36,   418,     0,     0,    26,    27,    28,
      29,    30,   693,   267,   268,     0,     0,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,    35,    36,     0,     0,     0,     0,     0,     0,
       0,   299,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,    37,     0,
       0,     0,    38,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     7,   312,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,   787,     0,     0,     0,     0,   418,
     796,   639,    26,    27,    28,    29,    30,   812,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
     463,   464,   465,   466,   467,     0,   468,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,    35,    36,     0,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,    37,     0,     0,     0,    38,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,   931,   312,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,   788,
       0,     0,     0,     0,     0,     0,   645,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
     463,   464,   465,   466,   467,     0,   468,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    37,    17,     0,    19,    38,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,   938,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,   463,   464,   465,   466,   467,
       0,   468,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,   979,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,     0,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,   511,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
     463,   464,   465,   466,   467,     0,   468,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,   463,   464,   465,   466,   467,
       0,   468,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   627,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   629,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,   463,   464,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   631,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   633,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,   463,   464,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,   472,   473,
     474,   475,   476,   477,   478,   479,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   637,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   639,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   641,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   643,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   645,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   647,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   649,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   651,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   653,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   655,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   657,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   659,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,     0,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,   678,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,   785,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   793,    26,    27,    28,
      29,    30,     0,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     271,   272,   273,     0,     0,   252,   274,   275,   184,   253,
       0,     0,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,     0,   267,   268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
     269,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   271,   272,   273,     0,     0,
     252,   274,   275,   184,   253,     0,     0,     0,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   266,     0,
       0,     0,     0,     0,     0,     0,   655,    26,    27,    28,
      29,    30,   501,   267,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,   269,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
     465,   466,   467,     0,   468,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    38,     0,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     271,   272,     0,     0,     0,   252,   274,   275,   184,   253,
       0,     7,     0,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   266,     7,     0,     7,     0,     0,   310,
       0,     0,    26,    27,    28,    29,    30,   142,   267,   268,
       0,     0,     0,     7,     0,     7,     0,   157,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,     0,     0,
       0,     0,   359,     0,   359,  1051,     0,  1068,    34,     0,
     269,    35,    36,     0,     0,     0,     0,    35,    36,     7,
       0,   234,     0,   359,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,    37,     0,     0,
       0,    38,     0,    37,     0,     0,     0,    38,     0,     0,
      35,    36,    35,    36,     0,   495,     0,   432,   496,     0,
       0,   274,   275,   332,     0,     0,     0,   312,     0,    35,
      36,    35,    36,     0,  1041,     0,    37,     0,    37,     0,
     158,     0,   158,     0,     0,     0,     0,     0,   235,     0,
     235,   236,     0,   236,     0,    37,   161,    37,   161,   158,
     162,   158,   162,     0,     0,    35,    36,   235,     0,   235,
     236,     7,   236,     0,     0,   161,     0,   161,     0,   162,
       0,   162,    35,    36,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,   158,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,   236,     0,    37,   291,
       0,   161,   158,     7,     0,   162,     0,   142,     0,     0,
     235,     0,     0,   236,   618,     0,     0,   157,   161,     0,
       0,     0,   162,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,   291,     0,     0,     0,     0,     0,    35,    36,    25,
      26,    27,    28,    29,    30,     0,     0,     0,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,    37,     0,   619,     0,    38,   463,   464,
     465,   466,   467,     0,   468,   495,    34,     0,   496,    35,
      36,     0,     0,   332,     0,     0,     0,     0,   469,   620,
     471,   472,   621,   474,   475,   476,   477,   622,   479,   480,
       0,     0,     0,     0,     0,    37,     7,     0,     0,    38,
       0,     0,     0,     0,    39,  1127,     0,   292,     0,     0,
     293,     0,     0,     0,     0,   161,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,   291,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,    30,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,     0,   463,   464,   465,
     466,   467,     0,   468,     0,     0,     0,     0,     0,    34,
       0,     0,    35,    36,     0,     0,     0,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     7,
       0,     0,    38,     0,     0,     0,     0,    39,     0,     0,
     486,     0,     0,   487,     0,     0,     0,     0,   161,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,    25,    26,    27,    28,    29,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,     0,    35,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
      61,    37,  -260,    62,     0,    38,     0,    63,    64,    65,
      39,     0,     0,   412,     0,     0,   413,     0,     0,     0,
       0,   161,    66,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
       0,     0,     0,    67,    68,     0,     0,     0,     0,  -260,
    -260,  -260,  -260,  -260,     0,     0,     0,    69,     0,     0,
       0,     0,    70,    71,    72,    73,    74,    75,    76,  -260,
    -260,  -260,     0,     0,     0,     0,    77,    78,    79,     0,
      80,    81,    82,    83,    84,  -260,  -260,     0,  -260,  -260,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    61,  -260,  -260,    62,    97,  -260,     0,
      63,    64,    65,    98,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,     0,     0,     0,    67,    68,     0,   722,
       0,     0,  -260,  -260,  -260,  -260,  -260,     0,     0,     0,
      69,     0,     0,     0,     0,    70,    71,    72,    73,    74,
      75,    76,  -260,  -260,  -260,     0,     0,     0,     0,    77,
      78,    79,     0,    80,    81,    82,    83,    84,  -260,  -260,
       0,  -260,  -260,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    61,  -260,  -260,    62,
      97,  -260,     0,    63,    64,    65,    98,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,     0,     0,     0,    67,
      68,     0,   817,     0,     0,  -260,  -260,  -260,  -260,  -260,
       0,     0,     0,    69,     0,     0,     0,     0,    70,    71,
      72,    73,    74,    75,    76,  -260,  -260,  -260,     0,     0,
       0,     0,    77,    78,    79,     0,    80,    81,    82,    83,
      84,  -260,  -260,     0,  -260,  -260,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    61,
    -260,  -260,    62,    97,  -260,     0,    63,    64,    65,    98,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,     0,
       0,     0,    67,    68,     0,   828,     0,     0,  -260,  -260,
    -260,  -260,  -260,     0,     0,     0,    69,     0,     0,     0,
       0,    70,    71,    72,    73,    74,    75,    76,  -260,  -260,
    -260,     0,     0,     0,     0,    77,    78,    79,     0,    80,
      81,    82,    83,    84,  -260,  -260,     0,  -260,  -260,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    61,  -260,  -260,    62,    97,  -260,     0,    63,
      64,    65,    98,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,  -260,
    -260,  -260,     0,     0,     0,    67,    68,     0,     0,     0,
       0,  -260,  -260,  -260,  -260,  -260,     0,     0,     0,    69,
       0,     0,     0,   976,    70,    71,    72,    73,    74,    75,
      76,  -260,  -260,  -260,     0,     0,     0,     0,    77,    78,
      79,     0,    80,    81,    82,    83,    84,  -260,  -260,     0,
    -260,  -260,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,     6,     0,     7,     0,     0,     0,
       0,     0,     0,     0,    96,     0,  -260,     0,     0,    97,
    -260,     0,     0,     0,     0,    98,    99,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
       0,     0,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     7,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,    38,     0,     0,     0,     0,    39,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,    25,    26,    27,    28,    29,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,    35,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,    38,     0,     0,     0,     0,    39,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,    25,    26,    27,    28,    29,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,     0,    35,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,     0,     0,     0,     0,     0,     0,     0,
      37,   371,     0,     0,    38,     0,     0,     0,     0,    39,
     534,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,    25,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    35,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,    38,     0,     0,
       0,     0,    39,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,     0,     0,     0,     0,     0,     0,     0,    25,    26,
      27,    28,    29,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,     0,    35,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,   400,    38,     0,
       0,     0,     0,    39,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,    25,
      26,    27,    28,    29,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      31,    32,    33,    63,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,    66,    35,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   891,
      68,     0,  -197,     0,     0,    37,     0,     0,     0,    38,
       0,     0,     0,     0,    39,     0,     0,     0,    70,    71,
      72,   892,    74,    75,    76,     0,     0,     0,   893,   894,
     895,     0,    77,    78,    79,     0,    80,    81,    82,    83,
      84,     0,     0,     0,     0,     0,    85,     0,     0,     0,
      89,    90,    91,    92,    93,    94,    95,   189,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,  1010,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,    27,    28,    29,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,     0,    35,    36,     0,     0,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,     0,
       0,    37,     0,     0,     0,    38,    26,    27,    28,    29,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,     0,
       0,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,   973,     0,    35,    36,     0,     0,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    37,     0,     0,     0,    38,   974,    25,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    35,    36,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,   212,
       0,     0,     0,    37,     0,     0,     0,    38,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    35,    36,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,    37,     0,     0,     0,    38,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,   241,     0,    35,    36,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,    37,     0,     0,     0,    38,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    35,    36,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
     222,    17,   223,    19,   224,    21,    22,    23,    24,     0,
       0,     0,     0,    37,     0,     0,     0,    38,   225,     0,
       0,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    35,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,    38,     8,     9,
      10,    11,    12,    13,    14,    15,   222,    17,   223,    19,
     224,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   225,     0,     0,    29,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,    35,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     0,     0,   972,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,    38,     8,     9,    10,    11,    12,    13,
      14,    15,   222,    17,   223,    19,   224,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,     0,     0,    29,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     7,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,     0,    35,
      36,     0,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,     0,     0,     0,    37,     0,     0,     0,    38,
       0,     0,     0,    29,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     7,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,     0,    35,
      36,     0,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,   222,    17,   223,    19,   224,    21,    22,    23,
      24,     0,     0,     0,     0,    37,     0,     0,     0,    38,
     225,     0,     0,    62,     0,     0,     0,    63,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,     0,    35,
      36,     0,     0,   891,    68,     0,  -200,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    37,    74,    75,    76,    38,
       0,     0,   893,   894,   895,     0,    77,    78,    79,     0,
      80,    81,    82,    83,    84,     0,     0,    62,     0,     0,
      85,    63,    64,    65,    89,    90,    91,    92,    93,    94,
      95,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   891,    68,     0,
    -198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,     0,
      74,    75,    76,     0,     0,     0,   893,   894,   895,     0,
      77,    78,    79,     0,    80,    81,    82,    83,    84,     0,
       0,     0,     0,     0,    85,     0,     0,     0,    89,    90,
      91,    92,    93,    94,    95,     0,     0,   681,     0,     0,
       0,     0,     0,     0,   448,     0,    96,   184,   253,     0,
       0,    97,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,     0,     0,     0,     0,     0,   683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   449,   463,   464,   465,   466,   467,     0,   468,
     450,     0,     0,     0,     0,     0,     0,   267,   268,     0,
       0,     0,   807,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   463,   464,   465,   466,   467,
       0,   468,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,     0,   463,   464,
     465,   466,   467,     0,   468,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480
};

static const yytype_int16 yycheck[] =
{
       1,     2,    32,    33,   156,    46,    59,   146,   185,    45,
       5,   226,    70,    97,    45,     5,   147,   557,    45,     1,
       2,     5,   574,    97,   595,   590,     5,   362,    91,     5,
     209,     5,   379,    91,    92,    38,   175,   176,    39,   541,
      45,    43,     5,   550,   541,     5,     5,     5,   550,    97,
     717,   820,   716,   386,   251,   234,     5,     5,    59,   981,
       6,   896,   682,   137,     3,     3,     6,    42,   183,    41,
      43,    48,    55,   188,     0,  1043,     5,    45,    63,   137,
      49,   196,    46,     5,     4,     6,     6,     3,    43,     6,
       3,    97,   117,    55,   428,     3,    46,   159,   160,   433,
      41,   185,     5,   218,     5,     5,    42,   169,     5,     5,
       5,   185,   153,     0,    76,     5,     5,   142,    44,   142,
     161,    61,   237,    48,    44,    46,    46,   154,    41,   159,
     160,   137,    43,   116,   111,  1103,    49,   185,    55,     0,
     125,   116,  1064,   340,   197,   198,   147,   111,   116,     3,
      89,    90,  1120,   154,   190,   158,     3,  1125,   130,   212,
     165,   111,   167,   135,   102,   147,     3,    43,   221,   124,
     125,  1093,   118,    89,    90,  1010,    89,    90,   214,   185,
     116,    89,    90,   214,  1106,   124,   111,   214,    44,     6,
      55,     4,   103,   540,   114,  1118,   197,   198,   171,   819,
     771,     0,   115,   119,    41,    55,   119,   234,    44,   102,
     362,   212,    49,   560,   127,   835,     4,   130,     6,     3,
     221,     3,   190,    55,  1147,   226,   139,    44,   120,   562,
     292,   293,    41,    46,   296,    89,    90,   102,     3,     1,
      76,     6,    89,    90,   226,   281,   102,    41,   124,   125,
     281,  1020,    89,    90,   281,    41,   292,   293,     3,    41,
     296,   115,   292,   293,   291,   119,   116,   602,   330,   331,
     102,   324,   119,   241,   327,   243,   281,    41,   115,    41,
       4,   135,   119,   310,     4,     3,     6,   954,   952,   342,
     291,   332,    44,   117,    41,     3,   337,   862,  1067,    41,
     330,   331,   139,    55,   991,    89,    90,    89,    90,   310,
      42,   818,     6,   281,   821,    43,   818,   290,   142,   821,
     378,   818,    46,   324,    89,    90,   327,    55,   386,   392,
     461,   115,   359,   115,   392,   119,     3,   119,    53,   104,
     105,   342,    41,   127,    89,    90,   130,     4,   321,     6,
     115,   135,    46,   135,   119,   360,    71,   139,     3,   574,
       3,   576,   127,     6,    72,   130,  1053,    41,  1055,   422,
     135,    89,    90,   118,   119,   103,   412,   413,   414,   432,
     127,    89,    90,   130,    92,   719,    41,   428,   135,    46,
     569,   418,   433,    41,    73,   127,    41,   115,   130,    41,
     405,   119,   749,   135,    49,   432,    55,   115,    41,    76,
      55,   119,     3,   978,    59,    89,    90,   751,    41,   446,
     972,   422,    89,    90,   486,   487,   488,    76,   543,     3,
     109,   432,  1136,   495,   496,   550,    59,   771,    41,   442,
      44,   503,    46,    46,    89,    90,    89,    90,   115,   502,
     602,    41,   119,  1157,   507,    46,   486,   487,   499,    49,
     461,   104,   105,    41,   526,   495,   496,    41,   499,    59,
     115,    41,   499,     6,   119,     3,    50,    51,    52,   461,
      41,   534,   127,   536,    41,   130,    55,    53,    54,    59,
     135,   825,   545,    41,   139,    41,    41,  1037,    89,    90,
      46,   502,    59,     3,   562,    71,   507,    42,    43,    41,
       3,    59,   565,    41,    59,    89,    90,     3,    44,   116,
      46,   557,    41,    42,   115,   578,   557,    59,   119,    41,
     557,   499,    42,   534,    49,   536,    41,    42,   873,   580,
      59,   115,    41,    42,   545,   119,  1117,    59,    41,   580,
      76,   552,   525,   580,    59,    55,    49,    44,   531,    46,
      59,    89,    90,   558,   565,   139,    59,    46,   558,   570,
     552,   561,     6,   574,   558,   576,   102,   578,     3,   558,
     127,   622,   558,   130,   558,   553,   763,   115,   570,    89,
      90,   119,   574,    46,   576,   558,    89,    90,   558,   558,
     558,   603,   127,    89,    90,   130,   668,   135,    44,   558,
     558,   139,   580,   581,    41,   115,    41,    44,   116,   119,
     772,    23,   115,   664,   597,   962,   119,    43,   965,   558,
      53,    54,   118,   119,   127,   676,   558,   130,    41,   680,
     671,   682,   135,    45,   671,   102,   139,   700,    71,    72,
      41,    50,    51,    52,    44,   558,    46,   558,   558,   116,
     117,   558,   558,   558,    89,    90,  1000,   725,   558,   558,
      41,    42,   103,     3,   727,    77,    78,    79,   719,   763,
      82,    43,    84,    85,    41,   142,    76,    42,    59,   763,
     115,     6,     7,  1027,   119,    41,    42,    42,   671,   700,
      90,   116,   755,   671,     6,     7,   701,   822,    59,   840,
     751,   701,    41,    59,   139,   763,   118,   701,  1052,   750,
     116,   873,   701,   750,     0,   701,   727,   701,   127,   724,
     771,   130,    42,    44,   724,    46,  1070,    44,   701,    46,
     724,   701,   701,   701,    55,   724,    40,  1082,   724,   717,
     724,    45,   701,   701,   755,  1089,   746,   972,    52,    89,
      90,   724,   746,   847,   724,   724,   724,   746,    43,   832,
     746,    55,   701,   847,   832,   724,   724,   750,   819,   701,
      89,    90,   750,  1117,   825,   115,    42,   789,     1,   119,
     843,    85,    86,    43,   835,   724,    42,   855,   701,   847,
     701,   701,   724,    97,   701,   701,   701,   770,   121,   122,
     123,   701,   701,     4,    55,     6,   789,  1152,    43,   872,
      43,   724,   116,   724,   724,   104,   105,   724,   724,   724,
      43,    44,    42,    46,   724,   724,    42,    41,   844,   840,
      42,   847,   843,   102,   996,    44,    50,    51,    52,   855,
      44,    55,    42,    44,   928,    46,   266,   267,   840,    42,
     154,   829,   857,    42,    55,   159,   160,   857,   162,    42,
     928,   872,   103,   857,    43,   169,    80,  1016,   857,    80,
     928,   857,    41,   857,    88,    89,    90,    88,    89,    90,
     116,   185,    44,    42,   857,   116,   190,   857,   857,   857,
      50,    51,    52,    46,   867,    76,   200,   201,   857,   857,
      42,     3,   880,   971,     3,   102,   974,   135,   136,   137,
     214,    23,   928,   127,    41,     3,   130,     3,   857,   982,
    1082,   225,   116,   227,   228,   857,   116,   231,    42,    42,
     234,   235,   236,    45,   949,   239,    41,   241,   242,   243,
     244,    73,    74,    75,   857,    49,   857,   857,    44,  1000,
     857,   857,   857,    44,    46,   970,    46,   857,   857,    42,
      44,   972,  1002,    41,    44,    77,    78,    79,    47,    91,
      82,   982,    84,    85,    42,   953,  1027,   281,  1041,    41,
     972,   133,   134,   135,   136,   137,    89,   291,   292,   293,
    1152,  1037,   296,    90,    42,   973,  1037,   975,    41,    76,
    1037,  1052,    78,  1040,  1041,    41,   310,   311,   312,    41,
      41,    49,   990,   317,   987,    42,    42,    42,   991,  1070,
    1083,  1036,    42,    42,   147,   329,   330,   331,    43,    42,
    1041,  1009,  1140,   337,   157,  1143,  1144,  1145,  1089,    55,
      70,    55,    44,   102,  1107,   349,   350,   142,  1156,    42,
    1113,  1114,  1115,    47,    41,   359,    86,   103,    42,    44,
      50,    51,    52,   367,  1104,   102,  1117,  1135,    44,    43,
    1133,    63,  1083,     3,    46,    42,   380,    42,  1129,    42,
    1053,    42,  1055,    42,    42,    42,    42,    42,    42,  1140,
      80,    68,  1143,  1144,  1145,    44,  1107,    44,    88,    89,
      90,    42,  1113,  1114,  1115,  1156,    76,    55,   412,   413,
     414,    41,    47,    90,   418,   419,   420,    55,    95,   423,
     424,   425,  1133,   427,   247,    44,    46,   125,   432,    47,
      76,  1109,    76,    76,     3,    47,    44,   127,    44,   103,
     130,    42,    55,   266,    43,    42,    44,   270,   271,   272,
     273,   274,   275,   183,    76,    76,    44,   187,   188,    89,
      90,    76,    76,    44,   194,   195,   196,    76,    44,   103,
      42,    44,    41,    43,    43,    49,   206,     3,    41,   209,
      49,    41,    47,    44,   488,   115,    55,    44,   218,   119,
      59,    41,    44,    42,    47,   499,    42,    41,    49,   503,
       3,    42,   325,     6,   234,   328,   183,   237,    42,   139,
     187,   188,   242,   517,    42,    41,    46,   521,   248,   196,
      89,    90,   526,   527,    42,   529,    42,    44,   746,  1109,
     576,   918,   209,    59,   103,   746,   138,    39,    41,   746,
     461,   218,   219,   840,   597,    50,   115,     3,   867,   553,
     119,   987,   612,   557,   986,   884,   233,   234,   127,  1003,
     237,   130,   680,    89,    90,  1117,   135,  1099,   245,   176,
     139,   726,    -1,    -1,    -1,    -1,   580,   581,   582,    -1,
      -1,    -1,   586,   587,   588,    41,    89,    90,    44,   115,
      -1,    -1,    -1,   119,   160,    -1,   110,    -1,    -1,    -1,
      -1,   127,    -1,   169,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,   115,   139,    -1,    -1,   119,   131,   132,   133,
     134,   135,   136,   137,   127,    -1,    -1,   130,    -1,    -1,
      -1,    -1,   135,    89,    90,    -1,   139,    -1,   461,    -1,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,    -1,   115,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   671,    -1,     3,
     236,   127,   676,   239,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,   686,   139,   688,   508,    -1,    -1,   692,   693,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,   717,     6,    -1,    50,    51,    52,    53,
      54,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,     3,    -1,    -1,    -1,    70,    71,    72,    -1,
      -1,   745,    -1,    -1,    -1,   311,   750,    -1,    -1,   753,
      -1,   317,    86,    87,    -1,    89,    90,    -1,    -1,   763,
      -1,    -1,    -1,   329,   330,   331,    -1,    -1,    -1,    -1,
      41,   337,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,   115,    -1,    -1,    -1,   119,   120,    -1,    59,    -1,
      -1,     3,     4,     3,     6,   618,   619,   620,   621,   622,
       3,   624,    -1,    -1,    -1,   139,    -1,   811,   812,    -1,
      -1,    -1,    -1,   543,    -1,    -1,    -1,    -1,    89,    90,
     550,    -1,    -1,    -1,    -1,   829,    -1,    -1,    -1,    41,
      -1,    41,    44,    -1,    46,    -1,    -1,   567,    41,   569,
      50,    51,    52,   847,   115,    -1,    49,    -1,   119,    59,
      -1,    -1,    -1,   419,   420,    -1,    59,   423,   424,   425,
     537,   427,    -1,    -1,   541,    -1,   543,    -1,   139,    -1,
      -1,    -1,   549,   550,    -1,    -1,   880,    89,    90,    89,
      90,    -1,    -1,    -1,   888,    -1,    89,    90,     3,    -1,
      -1,    -1,   569,    -1,   571,   625,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   581,   115,    -1,   119,    -1,   119,
      -1,    -1,   115,    -1,    -1,   127,   119,   127,   130,    -1,
     130,    -1,    -1,   135,   928,   135,    41,   139,    -1,   139,
      -1,    -1,     3,   756,    49,    -1,   139,   503,    -1,     3,
      -1,    -1,    -1,    -1,    59,     3,    -1,    -1,    -1,   953,
     773,   517,    -1,    -1,    -1,   521,    -1,    -1,    -1,    -1,
     526,   527,    -1,   529,   787,   788,   970,   790,    -1,   973,
      41,   975,    -1,   796,    89,    90,    -1,    41,    49,    50,
      51,    52,    -1,    41,    -1,    49,   990,    -1,    -1,    -1,
      -1,    49,    -1,   997,    -1,   725,    -1,    -1,    -1,    -1,
     115,    59,    -1,    -1,   119,  1009,    -1,     3,    -1,    -1,
      -1,    -1,   127,    -1,    -1,   130,   582,   840,    89,    90,
     586,   587,   588,    -1,   139,    89,    90,    -1,    -1,    -1,
      -1,    89,    90,  1037,    -1,    -1,    -1,  1041,     3,    -1,
     717,    -1,    -1,    -1,   115,    41,    -1,    -1,   119,   726,
      -1,   115,    -1,    49,    -1,   119,   127,   115,    -1,   130,
      -1,   119,  1066,   127,    -1,    -1,   130,    -1,   139,   127,
      -1,   135,   130,    -1,    -1,   139,    41,   135,    -1,    -1,
      -1,   139,    -1,    -1,    49,   131,   132,   133,   134,   135,
     136,   137,   822,    89,    90,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1109,   836,   837,   838,    -1,
     676,    -1,   935,   936,    -1,    -1,    -1,    -1,    -1,   115,
     686,    -1,   688,   119,    89,    90,   692,   693,    -1,    -1,
      -1,   127,   106,   107,   130,    -1,   110,    -1,    -1,   869,
      -1,   818,    -1,   139,   821,   822,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,   119,    -1,    -1,   131,   132,   133,
     134,   135,   136,   137,    -1,     1,    -1,     3,    -1,   846,
       6,     7,    -1,     3,   139,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,   753,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    50,    51,    52,    53,    54,    49,
      56,    57,    -1,    59,    -1,    -1,    -1,  1050,    -1,    59,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   811,   812,    -1,    -1,  1072,
      86,    -1,    88,    89,    90,    -1,   986,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   954,    -1,   115,
      -1,    -1,    -1,   119,    -1,   115,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,   130,    -1,    -1,   133,   134,   135,
       1,    -1,     3,   139,   140,     6,     7,    -1,     3,   139,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    46,    41,    -1,    -1,    50,
      51,    52,    53,    54,    49,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    86,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,
     115,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,   130,
      -1,    -1,   133,   134,   135,     1,    -1,     3,   139,   140,
       6,     7,    -1,     3,   139,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    50,    51,    52,    53,    54,    49,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,   119,    -1,   115,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,   130,    -1,    -1,   133,   134,   135,
       1,    -1,     3,   139,   140,     6,     7,    -1,    42,   139,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,   106,   107,   108,   109,   110,    -1,   112,    -1,
     106,   107,   108,   109,   110,    86,    -1,    88,    89,    90,
      -1,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,   115,    -1,    -1,    -1,   119,     1,
      -1,     3,     4,    -1,    -1,    -1,     8,     9,    10,   130,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,   140,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    -1,    -1,   118,   119,   120,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,     6,     7,    -1,     3,   139,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    46,    41,    -1,    -1,    50,    51,    52,
      53,    54,    49,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,   115,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,     3,   139,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    49,    50,    51,    52,    53,    54,    49,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,     3,    89,    90,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,   115,    -1,    -1,
      -1,   119,    -1,   115,    -1,    -1,   124,   119,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,     3,   139,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    89,    90,    41,    -1,    -1,    50,    51,    52,
      53,    54,    49,    56,    57,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,   115,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,     3,   139,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    41,
      76,    49,    50,    51,    52,    53,    54,    49,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    -1,   112,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    89,    90,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    42,   139,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
     106,   107,   108,   109,   110,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,   115,    33,    -1,    35,   119,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    42,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,   106,   107,   108,   109,   110,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    42,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
     106,   107,   108,   109,   110,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,   106,   107,   108,   109,   110,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,   106,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,   106,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
     133,   134,   135,    -1,    -1,     3,   139,   140,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,   134,   135,    -1,    -1,
       3,   139,   140,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    60,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     133,   134,    -1,    -1,    -1,     3,   139,   140,     6,     7,
      -1,     3,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,     3,    -1,     3,    -1,    -1,    41,
      -1,    -1,    50,    51,    52,    53,    54,    49,    56,    57,
      -1,    -1,    -1,     3,    -1,     3,    -1,    59,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    41,    44,    -1,    44,    86,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    89,    90,     3,
      -1,    41,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,   115,    -1,    -1,
      -1,   119,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,
      89,    90,    89,    90,    -1,   127,    -1,    41,   130,    -1,
      -1,   139,   140,   135,    -1,    -1,    -1,   139,    -1,    89,
      90,    89,    90,    -1,    41,    -1,   115,    -1,   115,    -1,
     119,    -1,   119,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     127,   130,    -1,   130,    -1,   115,   135,   115,   135,   119,
     139,   119,   139,    -1,    -1,    89,    90,   127,    -1,   127,
     130,     3,   130,    -1,    -1,   135,    -1,   135,    -1,   139,
      -1,   139,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,   130,    -1,   115,    41,
      -1,   135,   119,     3,    -1,   139,    -1,    49,    -1,    -1,
     127,    -1,    -1,   130,    42,    -1,    -1,    59,   135,    -1,
      -1,    -1,   139,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    89,    90,    49,
      50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,   115,    -1,   103,    -1,   119,   106,   107,
     108,   109,   110,    -1,   112,   127,    86,    -1,   130,    89,
      90,    -1,    -1,   135,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,   115,     3,    -1,    -1,   119,
      -1,    -1,    -1,    -1,   124,    44,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,    -1,   135,    -1,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    -1,   106,   107,   108,
     109,   110,    -1,   112,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,     3,
      -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    -1,    -1,
     127,    -1,    -1,   130,    -1,    -1,    -1,    -1,   135,    -1,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,   115,     3,     4,    -1,   119,    -1,     8,     9,    10,
     124,    -1,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
      -1,   135,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,     1,   115,     3,     4,   118,   119,    -1,
       8,     9,    10,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    -1,    50,    51,    52,    53,    54,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,     1,   115,     3,     4,
     118,   119,    -1,     8,     9,    10,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    -1,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,     1,
     115,     3,     4,   118,   119,    -1,     8,     9,    10,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,     1,   115,     3,     4,   118,   119,    -1,     8,
       9,    10,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    54,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    -1,    -1,    -1,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,   124,   125,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    13,    -1,    -1,   119,    -1,    -1,    -1,    -1,   124,
     125,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,   124,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,   124,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,
      70,    71,    72,     8,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    23,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    -1,   115,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    -1,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,   124,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,   119,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,   115,    -1,    -1,    -1,   119,   120,    49,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,    90,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    50,    -1,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,   119,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,   119,
      50,    -1,    -1,     4,    -1,    -1,    -1,     8,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    -1,    44,    45,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,   115,    67,    68,    69,   119,
      -1,    -1,    73,    74,    75,    -1,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    -1,    -1,     4,    -1,    -1,
      91,     8,     9,    10,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    -1,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,   113,     6,     7,    -1,
      -1,   118,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,   106,   107,   108,   109,   110,    -1,   112,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    60,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   106,   107,   108,   109,   110,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   106,   107,
     108,   109,   110,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   121,   122,   123,   144,   145,     1,     3,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    49,    50,    51,    52,    53,
      54,    70,    71,    72,    86,    89,    90,   115,   119,   124,
     194,   235,   236,   251,   252,   254,   255,   256,   259,   260,
     261,   289,   290,   305,   308,   310,     1,   236,     1,    41,
       0,     1,     4,     8,     9,    10,    23,    44,    45,    58,
      63,    64,    65,    66,    67,    68,    69,    77,    78,    79,
      81,    82,    83,    84,    85,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   113,   118,   124,   125,
     146,   147,   148,   150,   151,   152,   153,   156,   157,   159,
     160,   161,   162,   163,   164,   167,   168,   169,   172,   174,
     179,   180,   181,   182,   184,   188,   196,   197,   198,   199,
     200,   204,   205,   211,   212,   223,   224,   230,   231,   232,
      53,    71,    49,   280,   280,    41,   142,   102,   304,   235,
     308,     0,   252,   255,    41,    49,    55,    59,   119,   127,
     130,   135,   139,   240,   241,   243,   245,   246,   247,   248,
     308,   251,   260,   308,   304,   117,   142,   309,     0,   233,
     234,   236,   120,    41,     6,   118,   314,    41,   317,     1,
     253,   254,   305,   317,    41,   166,    41,    41,    41,    41,
      72,    92,   308,    46,   305,   308,    41,     4,    46,    41,
       4,   314,    41,   178,   253,   176,   177,   178,    41,    41,
     314,    41,    32,    34,    36,    50,   102,   187,   194,   256,
     259,   290,   308,   317,    41,   127,   130,   243,   245,   248,
     308,    87,   194,   253,   290,    45,   232,   257,   118,   119,
     310,   311,     3,     7,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    41,    56,    57,    88,
     130,   133,   134,   135,   139,   140,   236,   237,   238,   239,
     253,   254,   274,   275,   276,   277,   278,   279,   314,   315,
     251,    41,   127,   130,   233,   246,   248,   308,    46,   104,
     105,   262,   263,   264,   265,   274,   277,    60,   274,     3,
      41,    49,   139,   244,   247,   280,   308,    49,   244,   247,
     280,   251,   308,   240,    41,    59,   240,    41,    59,    49,
     127,   130,   135,   244,   247,   280,   308,   116,   310,   119,
     311,    42,    43,   124,   125,   263,   305,   306,   314,   187,
     290,     6,    46,   306,   318,   319,   306,     0,    44,    41,
     243,   245,    55,    53,    54,    71,   291,   292,   305,   305,
     306,    13,   173,   233,   233,   308,   308,    44,    55,   214,
      55,    46,   305,   175,   318,   233,    46,   242,   243,   245,
     246,    44,    43,   306,   307,   154,   155,   314,   233,   308,
     118,   208,   209,   210,   236,   289,   308,   308,   308,   127,
     130,   314,   127,   130,   248,   305,   308,   318,    41,    49,
      49,   306,    41,    49,   127,   130,   308,   116,   243,   308,
     266,   305,    41,   243,    76,   281,   282,   308,   314,     1,
     258,   274,   305,   309,    48,   111,   274,   276,     3,    41,
      49,   276,    41,    41,   274,   274,   274,   274,   274,   274,
     103,    43,    41,   106,   107,   108,   109,   110,   112,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    48,   111,     7,     6,     7,   127,   130,   248,   308,
     245,   308,   245,    42,    42,   127,   130,   245,   308,   116,
      59,    60,    41,   248,   308,   244,   308,    41,    59,   244,
     233,    60,   274,   233,    60,   274,   244,    49,   244,   247,
     280,    49,   244,   247,   280,   251,   116,    49,   127,   130,
     244,   251,   309,     0,   125,   233,    43,    42,   308,   308,
     183,    55,    42,    43,   240,    41,   262,   308,    42,    55,
      43,   171,    43,    42,    42,    44,    44,   253,   145,   308,
     213,    42,    42,    42,   176,    41,   178,    42,    42,    43,
      46,    42,   127,   130,   102,   103,    43,   240,    41,    61,
     116,    42,   248,   308,    44,   233,    49,    49,   116,    80,
      88,    89,    90,   127,   130,   249,   250,   251,   294,   297,
     298,   299,    55,    76,   195,   233,   308,   299,   283,    46,
      44,    90,   296,    42,    42,   304,     3,     3,    42,   103,
     127,   130,   135,   248,   102,    41,   239,    49,   274,    49,
     274,    49,   274,    49,   274,   274,   274,    49,   274,    49,
     274,    49,   274,    49,   274,    49,   274,    49,   274,    49,
     274,    49,   274,    49,   274,    49,   274,    49,   274,    49,
     274,     3,     3,   308,   116,    42,    42,    42,   116,   243,
     246,   251,   233,   244,   308,    42,   116,   233,    60,   274,
      42,    60,    42,    60,   244,   244,    49,   127,   130,   244,
     247,   244,    49,    49,   244,   233,   233,     4,    46,   314,
      41,   145,   154,   278,   314,   320,   306,    44,   233,    44,
      46,     4,   165,   314,   306,   314,   320,    42,   236,   243,
      46,   242,    47,    44,   145,   230,   176,    41,    47,   233,
     115,   119,   305,   312,    44,   318,   236,     4,    44,    46,
     114,   170,   314,   316,   208,    91,   206,   210,   233,   158,
     251,   243,   314,   116,    42,    41,    41,   294,    90,    89,
     297,   250,    46,   111,     1,    59,   189,   262,   255,    44,
      46,    42,    55,    76,   186,   240,    78,   284,   285,   293,
     202,   281,    41,    41,   274,    49,   274,    42,    42,   251,
      42,   127,   130,    49,   274,   305,    76,    41,    41,    42,
      42,   243,   246,    42,    42,   244,    42,    60,   298,   249,
     244,    49,    49,   244,   244,   244,   233,    47,    55,    42,
     149,    55,    43,   171,   170,   243,   299,    44,    47,   253,
     305,    44,    55,   316,   233,    42,   142,   117,   142,   313,
     102,    42,    47,    41,   103,   308,    45,   118,   184,   200,
     204,   205,   207,   220,   222,   230,    42,   145,   299,    44,
     233,   274,   187,    44,   102,   190,    44,   152,   267,   268,
     269,   270,    41,    55,   297,   299,   300,     1,   274,     1,
      43,    44,    46,   185,    43,    73,    74,    75,   286,   288,
       1,    44,    66,    73,    74,    75,   124,   150,   151,   152,
     153,   156,   157,   161,   164,   167,   169,   172,   174,   179,
     180,   181,   182,   200,   204,   205,   211,   215,   216,   217,
     218,   220,   221,   222,   223,   225,   226,   228,   230,    46,
      42,    42,   274,   274,   274,    42,    42,    42,    42,    42,
     274,    42,    42,    42,   244,   244,    42,   154,   320,   249,
     215,   314,   320,    42,   299,    44,   243,    44,   178,    42,
     249,   119,   305,   305,   119,   305,   237,     4,    46,   233,
     290,    55,   102,    87,   120,   253,    62,    44,    42,    42,
     294,   295,    41,    46,   191,   267,   124,   271,   272,   305,
      47,    43,   124,   125,   233,   262,    55,    76,   301,    44,
     243,   285,   308,   287,    46,    76,    76,    76,   217,    47,
     124,   218,   125,    32,    34,    36,   139,   229,   259,   308,
     201,   274,   274,   240,    47,   171,    44,   243,   170,    44,
      44,   313,   313,   103,    42,   240,   308,   253,   208,   308,
     253,    41,   294,   186,   233,    41,    44,   192,   272,   268,
      55,    44,   243,   125,   269,    43,    42,    44,   263,   302,
     303,   308,    44,    46,   299,   280,   293,   219,    44,   203,
     243,   310,    76,    44,   215,    44,   299,    44,    44,   240,
     242,   103,    55,    41,   127,   130,   248,   233,   185,    42,
     193,   273,   274,   299,   269,   269,    44,    46,    44,    43,
      49,    41,    46,   186,   308,   215,   299,    41,   274,    47,
      44,    44,   233,    41,    41,    41,   130,    42,   299,    44,
     186,   303,   185,   280,    47,   186,   233,    44,   203,    42,
     233,   233,   233,    41,   300,   111,   191,   185,    49,   185,
      42,   227,   299,    42,    42,    42,   233,   253,   192,   227,
      44,    46,    55,   227,   227,   227,    42,   191,   262,   227,
     192,    44
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   143,   144,   144,   144,   144,   144,   144,   144,   145,
     145,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   149,
     148,   150,   151,   152,   152,   152,   152,   152,   153,   153,
     154,   155,   155,   156,   156,   156,   158,   157,   159,   159,
     160,   160,   161,   161,   161,   161,   162,   163,   163,   164,
     164,   165,   165,   166,   166,   167,   167,   167,   168,   168,
     169,   169,   169,   169,   170,   170,   170,   171,   171,   172,
     173,   173,   174,   174,   174,   175,   176,   177,   177,   178,
     178,   178,   179,   180,   181,   182,   182,   182,   183,   182,
     182,   182,   182,   182,   184,   184,   184,   184,   184,   184,
     185,   185,   185,   185,   186,   186,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   188,   188,   188,
     189,   190,   190,   191,   192,   193,   192,   194,   194,   194,
     195,   195,   196,   197,   197,   198,   199,   199,   199,   199,
     199,   199,   201,   200,   202,   200,   203,   203,   204,   206,
     205,   205,   205,   205,   205,   207,   207,   207,   207,   207,
     207,   208,   208,   209,   209,   210,   210,   210,   210,   211,
     211,   211,   213,   212,   214,   212,   212,   215,   215,   215,
     215,   215,   216,   216,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   218,   218,   218,   219,   218,   220,   221,
     222,   222,   222,   222,   222,   223,   224,   225,   225,   225,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   227,   228,   229,   229,   229,   229,   229,   230,
     230,   231,   231,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   234,   234,   234,   234,   234,   234,   234,
     235,   235,   236,   236,   237,   237,   238,   238,   239,   239,
     240,   240,   240,   240,   241,   241,   241,   241,   242,   242,
     242,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   248,   248,
     248,   249,   249,   249,   250,   250,   251,   251,   252,   252,
     252,   253,   254,   254,   254,   254,   254,   255,   255,   255,
     255,   255,   255,   255,   255,   257,   256,   258,   258,   259,
     260,   260,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   262,   262,   263,   263,
     264,   265,   266,   266,   267,   267,   268,   268,   269,   269,
     269,   269,   269,   269,   269,   270,   271,   271,   272,   272,
     273,   274,   274,   275,   275,   275,   275,   275,   275,   275,
     275,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   280,   280,   281,   283,
     282,   282,   284,   284,   286,   285,   287,   285,   288,   288,
     288,   289,   289,   290,   290,   290,   291,   291,   291,   292,
     292,   293,   293,   294,   294,   294,   294,   295,   295,   296,
     296,   297,   297,   297,   297,   297,   297,   298,   298,   298,
     299,   299,   300,   300,   300,   300,   300,   300,   301,   301,
     302,   302,   302,   302,   303,   303,   304,   305,   305,   305,
     306,   306,   306,   307,   307,   308,   308,   308,   308,   308,
     308,   308,   309,   309,   309,   309,   310,   310,   311,   311,
     312,   312,   312,   312,   312,   312,   313,   313,   313,   313,
     314,   314,   315,   315,   315,   315,   316,   316,   316,   317,
     317,   318,   319,   319,   319,   319,   319,   319,   320,   320
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     2,     3,     2,     5,     2,     2,
       2,     2,     0,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       8,     5,     3,     5,     5,     9,     3,     3,     2,     2,
       4,     1,     1,     7,     7,     5,     0,     7,     1,     1,
       2,     2,     1,     5,     5,     5,     3,     7,     8,     5,
       3,     1,     1,     3,     0,     4,     7,     6,     1,     1,
       8,    10,     6,     8,     1,     1,     5,     5,     0,     7,
       1,     3,     6,     6,     8,     1,     1,     1,     3,     2,
       3,     6,     5,     9,     2,     1,     1,     1,     0,     6,
       1,     6,    10,     1,     6,     7,     9,     5,     6,     6,
       1,     5,     1,     1,     1,     2,     1,     1,     1,     2,
       1,     1,     2,     2,     3,     3,     1,    12,    14,     8,
       1,     1,     0,     1,     1,     0,     3,     1,     2,     2,
       2,     0,     5,     8,    11,     6,     1,     1,     1,     1,
       1,     1,     0,    10,     0,     8,     1,     4,     4,     0,
       6,     3,     6,     4,     7,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     3,     2,     7,     6,     1,     3,
       4,     4,     0,     6,     0,     5,     5,     1,     2,     0,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     0,     5,     6,     7,
       8,     8,     8,     9,     7,     5,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     2,     5,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     4,     4,     0,
       2,     1,     1,     2,     1,     0,     1,     3,     1,     1,
       2,     3,     2,     0,     2,     2,     1,     5,     1,     1,
       5,     0,     2,     3,     3,     1,     2,     2,     3,     4,
       5,     4,     3,     4,     4,     3,     3,     4,     5,     6,
       6,     5,     5,     1,     2,     3,     4,     5,     3,     4,
       4,     1,     2,     4,     4,     4,     5,     6,     5,     6,
       3,     4,     4,     5,     2,     2,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     2,     3,     3,     4,     3,
       4,     2,     3,     3,     4,     5,     3,     3,     2,     2,
       1,     1,     2,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     2,     1,     2,     3,     2,     1,     1,     1,
       2,     1,     1,     2,     1,     0,     4,     2,     2,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     1,     0,     1,     2,
       2,     3,     4,     4,     1,     3,     1,     2,     1,     3,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       5,     2,     1,     1,     3,     4,     5,     5,     5,     6,
       6,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     5,     3,     3,     3,
       5,     2,     2,     2,     2,     2,     1,     0,     1,     0,
       3,     0,     1,     3,     0,     4,     0,     6,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     1,     2,     2,     1,     0,     1,
       0,     4,     1,     1,     5,     2,     4,     1,     1,     2,
       1,     0,     3,     3,     4,     4,     3,     4,     2,     0,
       1,     3,     2,     4,     2,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     0,     2,     4,     1,     3,     1,
       2,     3,     3,     2,     2,     2,     1,     2,     1,     3,
       2,     4,     1,     3,     1,     3,     3,     2,     2,     2,
       2,     1,     2,     2,     2,     1,     1,     1,     1,     3,
       0,     1,     3,     5,     1,     3,     3,     5,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= END)
    {
      yychar = END;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: interface  */
#line 1958 "./CParse/parser.y"
                            {
                   if (!classes) classes = NewHash();
		   Setattr((yyvsp[0].node),"classes",classes); 
		   Setattr((yyvsp[0].node),"name",ModuleName);
		   
		   if ((!module_node) && ModuleName) {
		     module_node = new_node("module");
		     Setattr(module_node,"name",ModuleName);
		   }
		   Setattr((yyvsp[0].node),"module",module_node);
	           top = (yyvsp[0].node);
               }
#line 5492 "CParse/parser.c"
    break;

  case 3: /* program: PARSETYPE parm END  */
#line 1970 "./CParse/parser.y"
                                    {
                 top = Copy(Getattr((yyvsp[-1].p),"type"));
		 Delete((yyvsp[-1].p));
               }
#line 5501 "CParse/parser.c"
    break;

  case 4: /* program: PARSETYPE error  */
#line 1974 "./CParse/parser.y"
                                 {
                 top = 0;
               }
#line 5509 "CParse/parser.c"
    break;

  case 5: /* program: PARSEPARM parm END  */
#line 1977 "./CParse/parser.y"
                                    {
                 top = (yyvsp[-1].p);
               }
#line 5517 "CParse/parser.c"
    break;

  case 6: /* program: PARSEPARM error  */
#line 1980 "./CParse/parser.y"
                                 {
                 top = 0;
               }
#line 5525 "CParse/parser.c"
    break;

  case 7: /* program: PARSEPARMS LPAREN parms RPAREN END  */
#line 1983 "./CParse/parser.y"
                                                    {
                 top = (yyvsp[-2].pl);
               }
#line 5533 "CParse/parser.c"
    break;

  case 8: /* program: PARSEPARMS error  */
#line 1986 "./CParse/parser.y"
                                  {
                 top = 0;
               }
#line 5541 "CParse/parser.c"
    break;

  case 9: /* interface: interface declaration  */
#line 1991 "./CParse/parser.y"
                                           {  
                   /* add declaration to end of linked list (the declaration isn't always a single declaration, sometimes it is a linked list itself) */
                   if (currentDeclComment != NULL) {
		     set_comment((yyvsp[0].node), currentDeclComment);
		     currentDeclComment = NULL;
                   }                                      
                   appendChild((yyvsp[-1].node),(yyvsp[0].node));
                   (yyval.node) = (yyvsp[-1].node);
               }
#line 5555 "CParse/parser.c"
    break;

  case 10: /* interface: interface DOXYGENSTRING  */
#line 2000 "./CParse/parser.y"
                                             {
		   Delete(currentDeclComment);
                   currentDeclComment = (yyvsp[0].str); 
                   (yyval.node) = (yyvsp[-1].node);
               }
#line 5565 "CParse/parser.c"
    break;

  case 11: /* interface: interface DOXYGENPOSTSTRING  */
#line 2005 "./CParse/parser.y"
                                                 {
                   Node *node = lastChild((yyvsp[-1].node));
                   if (node) {
                     set_comment(node, (yyvsp[0].str));
		   } else {
		     Delete((yyvsp[0].str));
		   }
                   (yyval.node) = (yyvsp[-1].node);
               }
#line 5579 "CParse/parser.c"
    break;

  case 12: /* interface: %empty  */
#line 2014 "./CParse/parser.y"
                        {
                   (yyval.node) = new_node("top");
               }
#line 5587 "CParse/parser.c"
    break;

  case 16: /* declaration: SEMI  */
#line 2022 "./CParse/parser.y"
                      { (yyval.node) = 0; }
#line 5593 "CParse/parser.c"
    break;

  case 17: /* declaration: error  */
#line 2023 "./CParse/parser.y"
                       {
		  if (cparse_unknown_directive) {
		      Swig_error(cparse_file, cparse_line, "Unknown directive '%s'.\n", cparse_unknown_directive);
		  } else {
		      Swig_error(cparse_file, cparse_line, "Syntax error in input(1).\n");
		  }
		  Exit(EXIT_FAILURE);
               }
#line 5606 "CParse/parser.c"
    break;

  case 18: /* declaration: c_constructor_decl  */
#line 2032 "./CParse/parser.y"
                                    { 
                  if ((yyval.node)) {
   		      add_symbols((yyval.node));
                  }
                  (yyval.node) = (yyvsp[0].node); 
	       }
#line 5617 "CParse/parser.c"
    break;

  case 19: /* declaration: error CONVERSIONOPERATOR  */
#line 2048 "./CParse/parser.y"
                                          {
                  (yyval.node) = 0;
		  Delete((yyvsp[0].str));
                  skip_decl();
               }
#line 5627 "CParse/parser.c"
    break;

  case 39: /* $@1: %empty  */
#line 2084 "./CParse/parser.y"
                                                             {
               Node *cls;
	       String *clsname;
	       extendmode = 1;
	       cplus_mode = CPLUS_PUBLIC;
	       if (!classes) classes = NewHash();
	       if (!classes_typedefs) classes_typedefs = NewHash();
	       clsname = make_class_name((yyvsp[-1].str));
	       cls = Getattr(classes,clsname);
	       if (!cls) {
	         cls = Getattr(classes_typedefs, clsname);
		 if (!cls) {
		   /* No previous definition. Create a new scope */
		   Node *am = Getattr(Swig_extend_hash(),clsname);
		   if (!am) {
		     Swig_symbol_newscope();
		     Swig_symbol_setscopename((yyvsp[-1].str));
		     prev_symtab = 0;
		   } else {
		     prev_symtab = Swig_symbol_setscope(Getattr(am,"symtab"));
		   }
		   current_class = 0;
		 } else {
		   /* Previous typedef class definition.  Use its symbol table.
		      Deprecated, just the real name should be used. 
		      Note that %extend before the class typedef never worked, only %extend after the class typedef. */
		   prev_symtab = Swig_symbol_setscope(Getattr(cls, "symtab"));
		   current_class = cls;
		   SWIG_WARN_NODE_BEGIN(cls);
		   Swig_warning(WARN_PARSE_EXTEND_NAME, cparse_file, cparse_line, "Deprecated %%extend name used - the %s name '%s' should be used instead of the typedef name '%s'.\n", Getattr(cls, "kind"), SwigType_namestr(Getattr(cls, "name")), (yyvsp[-1].str));
		   SWIG_WARN_NODE_END(cls);
		 }
	       } else {
		 /* Previous class definition.  Use its symbol table */
		 prev_symtab = Swig_symbol_setscope(Getattr(cls,"symtab"));
		 current_class = cls;
	       }
	       Classprefix = NewString((yyvsp[-1].str));
	       Namespaceprefix= Swig_symbol_qualifiedscopename(0);
	       Delete(clsname);
	     }
#line 5673 "CParse/parser.c"
    break;

  case 40: /* extend_directive: EXTEND options classkeyopt idcolon LBRACE $@1 cpp_members RBRACE  */
#line 2124 "./CParse/parser.y"
                                  {
               String *clsname;
	       extendmode = 0;
               (yyval.node) = new_node("extend");
	       Setattr((yyval.node),"symtab",Swig_symbol_popscope());
	       if (prev_symtab) {
		 Swig_symbol_setscope(prev_symtab);
	       }
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
               clsname = make_class_name((yyvsp[-4].str));
	       Setattr((yyval.node),"name",clsname);

	       mark_nodes_as_extend((yyvsp[-1].node));
	       if (current_class) {
		 /* We add the extension to the previously defined class */
		 appendChild((yyval.node), (yyvsp[-1].node));
		 appendChild(current_class,(yyval.node));
	       } else {
		 /* We store the extensions in the extensions hash */
		 Node *am = Getattr(Swig_extend_hash(),clsname);
		 if (am) {
		   /* Append the members to the previous extend methods */
		   appendChild(am, (yyvsp[-1].node));
		 } else {
		   appendChild((yyval.node), (yyvsp[-1].node));
		   Setattr(Swig_extend_hash(),clsname,(yyval.node));
		 }
	       }
	       current_class = 0;
	       Delete(Classprefix);
	       Delete(clsname);
	       Classprefix = 0;
	       prev_symtab = 0;
	       (yyval.node) = 0;

	     }
#line 5714 "CParse/parser.c"
    break;

  case 41: /* apply_directive: APPLY typemap_parm LBRACE tm_list RBRACE  */
#line 2166 "./CParse/parser.y"
                                                           {
                    (yyval.node) = new_node("apply");
                    Setattr((yyval.node),"pattern",Getattr((yyvsp[-3].p),"pattern"));
		    appendChild((yyval.node),(yyvsp[-1].p));
               }
#line 5724 "CParse/parser.c"
    break;

  case 42: /* clear_directive: CLEAR tm_list SEMI  */
#line 2176 "./CParse/parser.y"
                                     {
		 (yyval.node) = new_node("clear");
		 appendChild((yyval.node),(yyvsp[-1].p));
               }
#line 5733 "CParse/parser.c"
    break;

  case 43: /* constant_directive: CONSTANT identifier EQUAL definetype SEMI  */
#line 2190 "./CParse/parser.y"
                                                                {
		 SwigType *type = NewSwigType((yyvsp[-1].dtype).type);
		 if (Len(type) > 0) {
		   (yyval.node) = new_node("constant");
		   Setattr((yyval.node), "name", (yyvsp[-3].id));
		   Setattr((yyval.node), "type", type);
		   Setattr((yyval.node), "value", (yyvsp[-1].dtype).val);
		   if ((yyvsp[-1].dtype).stringval) Setattr((yyval.node), "stringval", (yyvsp[-1].dtype).stringval);
		   if ((yyvsp[-1].dtype).numval) Setattr((yyval.node), "numval", (yyvsp[-1].dtype).numval);
		   Setattr((yyval.node), "storage", "%constant");
		   SetFlag((yyval.node), "feature:immutable");
		   add_symbols((yyval.node));
		   Delete(type);
		 } else {
		   Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE, cparse_file, cparse_line, "Unsupported constant value (ignored)\n");
		   (yyval.node) = 0;
		 }
	       }
#line 5756 "CParse/parser.c"
    break;

  case 44: /* constant_directive: CONSTANT type declarator def_args SEMI  */
#line 2208 "./CParse/parser.y"
                                                        {
		 SwigType_push((yyvsp[-3].type), (yyvsp[-2].decl).type);
		 /* Sneaky callback function trick */
		 if (SwigType_isfunction((yyvsp[-3].type))) {
		   SwigType_add_pointer((yyvsp[-3].type));
		 }
		 (yyval.node) = new_node("constant");
		 Setattr((yyval.node), "name", (yyvsp[-2].decl).id);
		 Setattr((yyval.node), "type", (yyvsp[-3].type));
		 Setattr((yyval.node), "value", (yyvsp[-1].dtype).val);
		 if ((yyvsp[-1].dtype).stringval) Setattr((yyval.node), "stringval", (yyvsp[-1].dtype).stringval);
		 if ((yyvsp[-1].dtype).numval) Setattr((yyval.node), "numval", (yyvsp[-1].dtype).numval);
		 Setattr((yyval.node), "storage", "%constant");
		 SetFlag((yyval.node), "feature:immutable");
		 add_symbols((yyval.node));
               }
#line 5777 "CParse/parser.c"
    break;

  case 45: /* constant_directive: CONSTANT type direct_declarator LPAREN parms RPAREN cv_ref_qualifier def_args SEMI  */
#line 2226 "./CParse/parser.y"
                                                                                                    {
		 SwigType_add_function((yyvsp[-7].type), (yyvsp[-4].pl));
		 SwigType_push((yyvsp[-7].type), (yyvsp[-2].dtype).qualifier);
		 SwigType_push((yyvsp[-7].type), (yyvsp[-6].decl).type);
		 /* Sneaky callback function trick */
		 if (SwigType_isfunction((yyvsp[-7].type))) {
		   SwigType_add_pointer((yyvsp[-7].type));
		 }
		 (yyval.node) = new_node("constant");
		 Setattr((yyval.node), "name", (yyvsp[-6].decl).id);
		 Setattr((yyval.node), "type", (yyvsp[-7].type));
		 Setattr((yyval.node), "value", (yyvsp[-1].dtype).val);
		 if ((yyvsp[-1].dtype).stringval) Setattr((yyval.node), "stringval", (yyvsp[-1].dtype).stringval);
		 if ((yyvsp[-1].dtype).numval) Setattr((yyval.node), "numval", (yyvsp[-1].dtype).numval);
		 Setattr((yyval.node), "storage", "%constant");
		 SetFlag((yyval.node), "feature:immutable");
		 add_symbols((yyval.node));
	       }
#line 5800 "CParse/parser.c"
    break;

  case 46: /* constant_directive: CONSTANT error SEMI  */
#line 2244 "./CParse/parser.y"
                                     {
		 Swig_warning(WARN_PARSE_BAD_VALUE,cparse_file,cparse_line,"Bad constant value (ignored).\n");
		 (yyval.node) = 0;
	       }
#line 5809 "CParse/parser.c"
    break;

  case 47: /* constant_directive: CONSTANT error END  */
#line 2248 "./CParse/parser.y"
                                    {
		 Swig_error(cparse_file,cparse_line,"Missing semicolon (';') after %%constant.\n");
		 Exit(EXIT_FAILURE);
	       }
#line 5818 "CParse/parser.c"
    break;

  case 48: /* echo_directive: ECHO HBLOCK  */
#line 2259 "./CParse/parser.y"
                             {
		 char temp[64];
		 Replace((yyvsp[0].str),"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace((yyvsp[0].str),"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", (yyvsp[0].str));
		 Delete((yyvsp[0].str));
                 (yyval.node) = 0;
	       }
#line 5832 "CParse/parser.c"
    break;

  case 49: /* echo_directive: ECHO string  */
#line 2268 "./CParse/parser.y"
                             {
		 char temp[64];
		 String *s = (yyvsp[0].str);
		 Replace(s,"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace(s,"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", s);
		 Delete(s);
                 (yyval.node) = 0;
               }
#line 5847 "CParse/parser.c"
    break;

  case 50: /* stringtype: string LBRACE parm RBRACE  */
#line 2281 "./CParse/parser.y"
                                          {		 
                 (yyval.node) = NewHash();
                 Setattr((yyval.node),"value",(yyvsp[-3].str));
		 Setattr((yyval.node),"type",Getattr((yyvsp[-1].p),"type"));
               }
#line 5857 "CParse/parser.c"
    break;

  case 51: /* fname: string  */
#line 2288 "./CParse/parser.y"
                       {
                 (yyval.node) = NewHash();
                 Setattr((yyval.node),"value",(yyvsp[0].str));
              }
#line 5866 "CParse/parser.c"
    break;

  case 53: /* fragment_directive: FRAGMENT LPAREN fname COMMA kwargs RPAREN HBLOCK  */
#line 2303 "./CParse/parser.y"
                                                                     {
                   Hash *p = (yyvsp[-2].node);
		   (yyval.node) = new_node("fragment");
		   Setattr((yyval.node),"value",Getattr((yyvsp[-4].node),"value"));
		   Setattr((yyval.node),"type",Getattr((yyvsp[-4].node),"type"));
		   Setattr((yyval.node),"section",Getattr(p,"name"));
		   Setattr((yyval.node),"kwargs",nextSibling(p));
		   Setattr((yyval.node),"code",(yyvsp[0].str));
		   Delete((yyvsp[0].str));
                 }
#line 5881 "CParse/parser.c"
    break;

  case 54: /* fragment_directive: FRAGMENT LPAREN fname COMMA kwargs RPAREN LBRACE  */
#line 2313 "./CParse/parser.y"
                                                                    {
		   Hash *p = (yyvsp[-2].node);
		   String *code;
		   if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		   (yyval.node) = new_node("fragment");
		   Setattr((yyval.node),"value",Getattr((yyvsp[-4].node),"value"));
		   Setattr((yyval.node),"type",Getattr((yyvsp[-4].node),"type"));
		   Setattr((yyval.node),"section",Getattr(p,"name"));
		   Setattr((yyval.node),"kwargs",nextSibling(p));
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   code = Copy(scanner_ccode);
		   Setattr((yyval.node),"code",code);
		   Delete(code);
                 }
#line 5901 "CParse/parser.c"
    break;

  case 55: /* fragment_directive: FRAGMENT LPAREN fname RPAREN SEMI  */
#line 2328 "./CParse/parser.y"
                                                     {
		   (yyval.node) = new_node("fragment");
		   Setattr((yyval.node),"value",Getattr((yyvsp[-2].node),"value"));
		   Setattr((yyval.node),"type",Getattr((yyvsp[-2].node),"type"));
		   Setattr((yyval.node),"emitonly","1");
		 }
#line 5912 "CParse/parser.c"
    break;

  case 56: /* @2: %empty  */
#line 2341 "./CParse/parser.y"
                                                             {
		     (yyval.loc).filename = Copy(cparse_file);
		     (yyval.loc).line = cparse_line;
		     scanner_set_location((yyvsp[-1].str),1);
                     if ((yyvsp[-2].node)) { 
		       String *maininput = Getattr((yyvsp[-2].node), "maininput");
		       if (maininput)
		         scanner_set_main_input_file(NewString(maininput));
		     }
               }
#line 5927 "CParse/parser.c"
    break;

  case 57: /* include_directive: includetype options string BEGINFILE @2 interface ENDOFFILE  */
#line 2350 "./CParse/parser.y"
                                          {
                     String *mname = 0;
                     (yyval.node) = (yyvsp[-1].node);
		     scanner_set_location((yyvsp[-2].loc).filename, (yyvsp[-2].loc).line + 1);
		     Delete((yyvsp[-2].loc).filename);
		     switch ((yyvsp[-6].includetype)) {
		       case INCLUDE_INCLUDE:
			 set_nodeType((yyval.node), "include");
			 break;
		       case INCLUDE_IMPORT:
			 mname = (yyvsp[-5].node) ? Getattr((yyvsp[-5].node), "module") : 0;
			 set_nodeType((yyval.node), "import");
			 if (import_mode) --import_mode;
			 break;
		     }
		     
		     Setattr((yyval.node),"name",(yyvsp[-4].str));
		     /* Search for the module (if any) */
		     {
			 Node *n = firstChild((yyval.node));
			 while (n) {
			     if (Strcmp(nodeType(n),"module") == 0) {
			         if (mname) {
				   Setattr(n,"name", mname);
				   mname = 0;
				 }
				 Setattr((yyval.node),"module",Getattr(n,"name"));
				 break;
			     }
			     n = nextSibling(n);
			 }
			 if (mname) {
			   /* There is no module node in the import
			      node, ie, you imported a .h file
			      directly.  We are forced then to create
			      a new import node with a module node.
			   */			      
			   Node *nint = new_node("import");
			   Node *mnode = new_node("module");
			   Setattr(mnode,"name", mname);
                           Setattr(mnode,"options",(yyvsp[-5].node));
			   appendChild(nint,mnode);
			   Delete(mnode);
			   appendChild(nint,firstChild((yyval.node)));
			   (yyval.node) = nint;
			   Setattr((yyval.node),"module",mname);
			 }
		     }
		     Setattr((yyval.node),"options",(yyvsp[-5].node));
               }
#line 5982 "CParse/parser.c"
    break;

  case 58: /* includetype: INCLUDE  */
#line 2402 "./CParse/parser.y"
                         { (yyval.includetype) = INCLUDE_INCLUDE; }
#line 5988 "CParse/parser.c"
    break;

  case 59: /* includetype: IMPORT  */
#line 2403 "./CParse/parser.y"
                         { (yyval.includetype) = INCLUDE_IMPORT; ++import_mode;}
#line 5994 "CParse/parser.c"
    break;

  case 60: /* inline_directive: INLINE HBLOCK  */
#line 2410 "./CParse/parser.y"
                                 {
                 String *cpps;
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "%%inline directive inside a namespace is disallowed.\n");
		   (yyval.node) = 0;
		 } else {
		   (yyval.node) = new_node("insert");
		   Setattr((yyval.node),"code",(yyvsp[0].str));
		   /* Need to run through the preprocessor */
		   Seek((yyvsp[0].str),0,SEEK_SET);
		   Setline((yyvsp[0].str),cparse_start_line);
		   Setfile((yyvsp[0].str),cparse_file);
		   cpps = Preprocessor_parse((yyvsp[0].str));
		   scanner_start_inline(cpps, cparse_start_line);
		   Delete(cpps);
		 }
		 Delete((yyvsp[0].str));
	       }
#line 6017 "CParse/parser.c"
    break;

  case 61: /* inline_directive: INLINE LBRACE  */
#line 2428 "./CParse/parser.y"
                               {
                 String *cpps;
		 int start_line = cparse_line;
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "%%inline directive inside a namespace is disallowed.\n");
		 }
		 if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		 if (Namespaceprefix) {
		   (yyval.node) = 0;
		 } else {
		   String *code;
                   (yyval.node) = new_node("insert");
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   code = Copy(scanner_ccode);
		   Setattr((yyval.node),"code", code);
		   Delete(code);		   
		   cpps=Copy(scanner_ccode);
		   scanner_start_inline(cpps, start_line);
		   Delete(cpps);
		 }
               }
#line 6044 "CParse/parser.c"
    break;

  case 62: /* insert_directive: HBLOCK  */
#line 2462 "./CParse/parser.y"
                          {
                 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"code",(yyvsp[0].str));
		 Delete((yyvsp[0].str));
	       }
#line 6054 "CParse/parser.c"
    break;

  case 63: /* insert_directive: INSERT LPAREN idstring RPAREN string  */
#line 2467 "./CParse/parser.y"
                                                      {
		 String *code = NewStringEmpty();
		 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"section",(yyvsp[-2].id));
		 Setattr((yyval.node),"code",code);
		 if (Swig_insert_file((yyvsp[0].str),code) < 0) {
		   Swig_error(cparse_file, cparse_line, "Couldn't find '%s'.\n", (yyvsp[0].str));
		   (yyval.node) = 0;
		 } 
               }
#line 6069 "CParse/parser.c"
    break;

  case 64: /* insert_directive: INSERT LPAREN idstring RPAREN HBLOCK  */
#line 2477 "./CParse/parser.y"
                                                      {
		 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"section",(yyvsp[-2].id));
		 Setattr((yyval.node),"code",(yyvsp[0].str));
		 Delete((yyvsp[0].str));
               }
#line 6080 "CParse/parser.c"
    break;

  case 65: /* insert_directive: INSERT LPAREN idstring RPAREN LBRACE  */
#line 2483 "./CParse/parser.y"
                                                      {
		 String *code;
		 if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"section",(yyvsp[-2].id));
		 Delitem(scanner_ccode,0);
		 Delitem(scanner_ccode,DOH_END);
		 code = Copy(scanner_ccode);
		 Setattr((yyval.node),"code", code);
		 Delete(code);
	       }
#line 6096 "CParse/parser.c"
    break;

  case 66: /* module_directive: MODULE options idstring  */
#line 2501 "./CParse/parser.y"
                                          {
                 (yyval.node) = new_node("module");
		 if ((yyvsp[-1].node)) {
		   Setattr((yyval.node),"options",(yyvsp[-1].node));
		   if (Getattr((yyvsp[-1].node),"directors")) {
		     Wrapper_director_mode_set(1);
		     if (!cparse_cplusplus) {
		       Swig_error(cparse_file, cparse_line, "Directors are not supported for C code and require the -c++ option\n");
		     }
		   } 
		   if (Getattr((yyvsp[-1].node),"dirprot")) {
		     Wrapper_director_protected_mode_set(1);
		   } 
		   if (Getattr((yyvsp[-1].node),"allprotected")) {
		     Wrapper_all_protected_mode_set(1);
		   } 
		   if (Getattr((yyvsp[-1].node),"templatereduce")) {
		     template_reduce = 1;
		   }
		   if (Getattr((yyvsp[-1].node),"notemplatereduce")) {
		     template_reduce = 0;
		   }
		 }
		 if (!ModuleName) ModuleName = NewString((yyvsp[0].id));
		 if (!import_mode) {
		   /* first module included, we apply global
		      ModuleName, which can be modify by -module */
		   String *mname = Copy(ModuleName);
		   Setattr((yyval.node),"name",mname);
		   Delete(mname);
		 } else { 
		   /* import mode, we just pass the idstring */
		   Setattr((yyval.node),"name",(yyvsp[0].id));   
		 }		 
		 if (!module_node) module_node = (yyval.node);
	       }
#line 6137 "CParse/parser.c"
    break;

  case 67: /* native_directive: NATIVE LPAREN identifier RPAREN storage_class identifier SEMI  */
#line 2544 "./CParse/parser.y"
                                                                                                  {
                 (yyval.node) = new_node("native");
		 Setattr((yyval.node),"name",(yyvsp[-4].id));
		 Setattr((yyval.node),"wrap:name",(yyvsp[-1].id));
		 Delete((yyvsp[-2].str));
	         add_symbols((yyval.node));
	       }
#line 6149 "CParse/parser.c"
    break;

  case 68: /* native_directive: NATIVE LPAREN identifier RPAREN storage_class type declarator SEMI  */
#line 2551 "./CParse/parser.y"
                                                                                    {
		 if (!SwigType_isfunction((yyvsp[-1].decl).type)) {
		   Swig_error(cparse_file,cparse_line,"%%native declaration '%s' is not a function.\n", (yyvsp[-1].decl).id);
		   (yyval.node) = 0;
		 } else {
		     Delete(SwigType_pop_function((yyvsp[-1].decl).type));
		     /* Need check for function here */
		     SwigType_push((yyvsp[-2].type),(yyvsp[-1].decl).type);
		     (yyval.node) = new_node("native");
	             Setattr((yyval.node),"name",(yyvsp[-5].id));
		     Setattr((yyval.node),"wrap:name",(yyvsp[-1].decl).id);
		     Setattr((yyval.node),"type",(yyvsp[-2].type));
		     Setattr((yyval.node),"parms",(yyvsp[-1].decl).parms);
		     Setattr((yyval.node),"decl",(yyvsp[-1].decl).type);
		 }
		 Delete((yyvsp[-3].str));
	         add_symbols((yyval.node));
	       }
#line 6172 "CParse/parser.c"
    break;

  case 69: /* pragma_directive: PRAGMA pragma_lang identifier EQUAL pragma_arg  */
#line 2578 "./CParse/parser.y"
                                                                  {
                 (yyval.node) = new_node("pragma");
		 Setattr((yyval.node),"lang",(yyvsp[-3].id));
		 Setattr((yyval.node),"name",(yyvsp[-2].id));
		 Setattr((yyval.node),"value",(yyvsp[0].str));
	       }
#line 6183 "CParse/parser.c"
    break;

  case 70: /* pragma_directive: PRAGMA pragma_lang identifier  */
#line 2584 "./CParse/parser.y"
                                              {
		(yyval.node) = new_node("pragma");
		Setattr((yyval.node),"lang",(yyvsp[-1].id));
		Setattr((yyval.node),"name",(yyvsp[0].id));
	      }
#line 6193 "CParse/parser.c"
    break;

  case 73: /* pragma_lang: LPAREN identifier RPAREN  */
#line 2595 "./CParse/parser.y"
                                         { (yyval.id) = (yyvsp[-1].id); }
#line 6199 "CParse/parser.c"
    break;

  case 74: /* pragma_lang: %empty  */
#line 2596 "./CParse/parser.y"
                       { (yyval.id) = "swig"; }
#line 6205 "CParse/parser.c"
    break;

  case 75: /* rename_directive: rename_namewarn declarator idstring SEMI  */
#line 2603 "./CParse/parser.y"
                                                            {
                SwigType *t = (yyvsp[-2].decl).type;
		Hash *kws = NewHash();
		String *fixname;
		fixname = feature_identifier_fix((yyvsp[-2].decl).id);
		Setattr(kws,"name",(yyvsp[-1].id));
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if ((yyvsp[-3].intvalue)) {
			Swig_name_rename_add(Namespaceprefix, nname,decl,kws,(yyvsp[-2].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,nname,decl,kws);
		      }
		      Delete(nname);
		    } else {
		      if ((yyvsp[-3].intvalue)) {
			Swig_name_rename_add(Namespaceprefix,(fixname),decl,kws,(yyvsp[-2].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,(fixname),decl,kws);
		      }
		    }
		    Delete(decl);
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if ((yyvsp[-3].intvalue)) {
		      Swig_name_rename_add(Namespaceprefix,(nname),0,kws,(yyvsp[-2].decl).parms);
		    } else {
		      Swig_name_namewarn_add(Namespaceprefix,(nname),0,kws);
		    }
		    Delete(nname);
		  }
		} else {
		  if ((yyvsp[-3].intvalue)) {
		    Swig_name_rename_add(Namespaceprefix,(fixname),0,kws,(yyvsp[-2].decl).parms);
		  } else {
		    Swig_name_namewarn_add(Namespaceprefix,(fixname),0,kws);
		  }
		}
                (yyval.node) = 0;
		scanner_clear_rename();
              }
#line 6256 "CParse/parser.c"
    break;

  case 76: /* rename_directive: rename_namewarn LPAREN kwargs RPAREN declarator cpp_const SEMI  */
#line 2649 "./CParse/parser.y"
                                                                               {
		String *fixname;
		Hash *kws = (yyvsp[-4].node);
		SwigType *t = (yyvsp[-2].decl).type;
		fixname = feature_identifier_fix((yyvsp[-2].decl).id);
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if ((yyvsp[-1].dtype).qualifier) SwigType_push(t,(yyvsp[-1].dtype).qualifier);
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if ((yyvsp[-6].intvalue)) {
			Swig_name_rename_add(Namespaceprefix, nname,decl,kws,(yyvsp[-2].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,nname,decl,kws);
		      }
		      Delete(nname);
		    } else {
		      if ((yyvsp[-6].intvalue)) {
			Swig_name_rename_add(Namespaceprefix,(fixname),decl,kws,(yyvsp[-2].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,(fixname),decl,kws);
		      }
		    }
		    Delete(decl);
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if ((yyvsp[-6].intvalue)) {
		      Swig_name_rename_add(Namespaceprefix,(nname),0,kws,(yyvsp[-2].decl).parms);
		    } else {
		      Swig_name_namewarn_add(Namespaceprefix,(nname),0,kws);
		    }
		    Delete(nname);
		  }
		} else {
		  if ((yyvsp[-6].intvalue)) {
		    Swig_name_rename_add(Namespaceprefix,(fixname),0,kws,(yyvsp[-2].decl).parms);
		  } else {
		    Swig_name_namewarn_add(Namespaceprefix,(fixname),0,kws);
		  }
		}
                (yyval.node) = 0;
		scanner_clear_rename();
              }
#line 6307 "CParse/parser.c"
    break;

  case 77: /* rename_directive: rename_namewarn LPAREN kwargs RPAREN string SEMI  */
#line 2695 "./CParse/parser.y"
                                                                 {
		if ((yyvsp[-5].intvalue)) {
		  Swig_name_rename_add(Namespaceprefix,(yyvsp[-1].str),0,(yyvsp[-3].node),0);
		} else {
		  Swig_name_namewarn_add(Namespaceprefix,(yyvsp[-1].str),0,(yyvsp[-3].node));
		}
		(yyval.node) = 0;
		scanner_clear_rename();
              }
#line 6321 "CParse/parser.c"
    break;

  case 78: /* rename_namewarn: RENAME  */
#line 2706 "./CParse/parser.y"
                         {
		    (yyval.intvalue) = 1;
                }
#line 6329 "CParse/parser.c"
    break;

  case 79: /* rename_namewarn: NAMEWARN  */
#line 2709 "./CParse/parser.y"
                           {
                    (yyval.intvalue) = 0;
                }
#line 6337 "CParse/parser.c"
    break;

  case 80: /* feature_directive: FEATURE LPAREN idstring featattr RPAREN declarator cpp_const stringbracesemi  */
#line 2736 "./CParse/parser.y"
                                                                                                 {
                    String *val = (yyvsp[0].str) ? NewString((yyvsp[0].str)) : NewString("1");
                    new_feature((yyvsp[-5].id), val, (yyvsp[-4].node), (yyvsp[-2].decl).id, (yyvsp[-2].decl).type, (yyvsp[-2].decl).parms, (yyvsp[-1].dtype).qualifier);
                    (yyval.node) = 0;
                    scanner_clear_rename();
                  }
#line 6348 "CParse/parser.c"
    break;

  case 81: /* feature_directive: FEATURE LPAREN idstring COMMA stringnum featattr RPAREN declarator cpp_const SEMI  */
#line 2742 "./CParse/parser.y"
                                                                                                      {
                    String *val = Len((yyvsp[-5].str)) ? (yyvsp[-5].str) : 0;
                    new_feature((yyvsp[-7].id), val, (yyvsp[-4].node), (yyvsp[-2].decl).id, (yyvsp[-2].decl).type, (yyvsp[-2].decl).parms, (yyvsp[-1].dtype).qualifier);
                    (yyval.node) = 0;
                    scanner_clear_rename();
                  }
#line 6359 "CParse/parser.c"
    break;

  case 82: /* feature_directive: FEATURE LPAREN idstring featattr RPAREN stringbracesemi  */
#line 2750 "./CParse/parser.y"
                                                                            {
                    String *val = (yyvsp[0].str) ? NewString((yyvsp[0].str)) : NewString("1");
                    new_feature((yyvsp[-3].id), val, (yyvsp[-2].node), 0, 0, 0, 0);
                    (yyval.node) = 0;
                    scanner_clear_rename();
                  }
#line 6370 "CParse/parser.c"
    break;

  case 83: /* feature_directive: FEATURE LPAREN idstring COMMA stringnum featattr RPAREN SEMI  */
#line 2756 "./CParse/parser.y"
                                                                                 {
                    String *val = Len((yyvsp[-3].str)) ? (yyvsp[-3].str) : 0;
                    new_feature((yyvsp[-5].id), val, (yyvsp[-2].node), 0, 0, 0, 0);
                    (yyval.node) = 0;
                    scanner_clear_rename();
                  }
#line 6381 "CParse/parser.c"
    break;

  case 85: /* stringbracesemi: SEMI  */
#line 2765 "./CParse/parser.y"
                       { (yyval.str) = 0; }
#line 6387 "CParse/parser.c"
    break;

  case 86: /* stringbracesemi: PARMS LPAREN parms RPAREN SEMI  */
#line 2766 "./CParse/parser.y"
                                                 { (yyval.str) = (yyvsp[-2].pl); }
#line 6393 "CParse/parser.c"
    break;

  case 87: /* featattr: COMMA idstring EQUAL stringnum featattr  */
#line 2769 "./CParse/parser.y"
                                                              {
		  (yyval.node) = NewHash();
		  Setattr((yyval.node),"name",(yyvsp[-3].id));
		  Setattr((yyval.node),"value",(yyvsp[-1].str));
		  if ((yyvsp[0].node)) set_nextSibling((yyval.node), (yyvsp[0].node));
		}
#line 6404 "CParse/parser.c"
    break;

  case 88: /* featattr: %empty  */
#line 2775 "./CParse/parser.y"
                         {
		  (yyval.node) = 0;
		}
#line 6412 "CParse/parser.c"
    break;

  case 89: /* varargs_directive: VARARGS LPAREN varargs_parms RPAREN declarator cpp_const SEMI  */
#line 2782 "./CParse/parser.y"
                                                                                  {
                 Parm *val;
		 String *name;
		 SwigType *t;
		 if (Namespaceprefix) name = NewStringf("%s::%s", Namespaceprefix, (yyvsp[-2].decl).id);
		 else name = NewString((yyvsp[-2].decl).id);
		 val = (yyvsp[-4].pl);
		 if ((yyvsp[-2].decl).parms) {
		   Setmeta(val,"parms",(yyvsp[-2].decl).parms);
		 }
		 t = (yyvsp[-2].decl).type;
		 if (!Len(t)) t = 0;
		 if (t) {
		   if ((yyvsp[-1].dtype).qualifier) SwigType_push(t,(yyvsp[-1].dtype).qualifier);
		   if (SwigType_isfunction(t)) {
		     SwigType *decl = SwigType_pop_function(t);
		     if (SwigType_ispointer(t)) {
		       String *nname = NewStringf("*%s",name);
		       Swig_feature_set(Swig_cparse_features(), nname, decl, "feature:varargs", val, 0);
		       Delete(nname);
		     } else {
		       Swig_feature_set(Swig_cparse_features(), name, decl, "feature:varargs", val, 0);
		     }
		     Delete(decl);
		   } else if (SwigType_ispointer(t)) {
		     String *nname = NewStringf("*%s",name);
		     Swig_feature_set(Swig_cparse_features(),nname,0,"feature:varargs",val, 0);
		     Delete(nname);
		   }
		 } else {
		   Swig_feature_set(Swig_cparse_features(),name,0,"feature:varargs",val, 0);
		 }
		 Delete(name);
		 (yyval.node) = 0;
              }
#line 6452 "CParse/parser.c"
    break;

  case 91: /* varargs_parms: NUM_INT COMMA parm  */
#line 2819 "./CParse/parser.y"
                                     { 
		  int i;
		  int n;
		  Parm *p;
		  n = atoi(Char((yyvsp[-2].dtype).val));
		  if (n <= 0) {
		    Swig_error(cparse_file, cparse_line,"Argument count in %%varargs must be positive.\n");
		    (yyval.pl) = 0;
		  } else {
		    String *name = Getattr((yyvsp[0].p), "name");
		    (yyval.pl) = Copy((yyvsp[0].p));
		    if (name)
		      Setattr((yyval.pl), "name", NewStringf("%s%d", name, n));
		    for (i = 1; i < n; i++) {
		      p = Copy((yyvsp[0].p));
		      name = Getattr(p, "name");
		      if (name)
		        Setattr(p, "name", NewStringf("%s%d", name, n-i));
		      set_nextSibling(p,(yyval.pl));
		      Delete((yyval.pl));
		      (yyval.pl) = p;
		    }
		  }
                }
#line 6481 "CParse/parser.c"
    break;

  case 92: /* typemap_directive: TYPEMAP LPAREN typemap_type RPAREN tm_list stringbrace  */
#line 2853 "./CParse/parser.y"
                                                                            {
		   (yyval.node) = 0;
		   if ((yyvsp[-3].tmap).method) {
		     String *code = 0;
		     (yyval.node) = new_node("typemap");
		     Setattr((yyval.node),"method",(yyvsp[-3].tmap).method);
		     if ((yyvsp[-3].tmap).kwargs) {
		       ParmList *kw = (yyvsp[-3].tmap).kwargs;
                       code = remove_block(kw, (yyvsp[0].str));
		       Setattr((yyval.node),"kwargs", (yyvsp[-3].tmap).kwargs);
		     }
		     code = code ? code : NewString((yyvsp[0].str));
		     Setattr((yyval.node),"code", code);
		     Delete(code);
		     appendChild((yyval.node),(yyvsp[-1].p));
		     Delete((yyvsp[-3].tmap).kwargs);
		     Delete((yyvsp[-3].tmap).method);
		   }
	       }
#line 6505 "CParse/parser.c"
    break;

  case 93: /* typemap_directive: TYPEMAP LPAREN typemap_type RPAREN tm_list SEMI  */
#line 2872 "./CParse/parser.y"
                                                                 {
		 (yyval.node) = 0;
		 if ((yyvsp[-3].tmap).method) {
		   (yyval.node) = new_node("typemap");
		   Setattr((yyval.node),"method",(yyvsp[-3].tmap).method);
		   appendChild((yyval.node),(yyvsp[-1].p));
		   Delete((yyvsp[-3].tmap).method);
		 }
		 Delete((yyvsp[-3].tmap).kwargs);
	       }
#line 6520 "CParse/parser.c"
    break;

  case 94: /* typemap_directive: TYPEMAP LPAREN typemap_type RPAREN tm_list EQUAL typemap_parm SEMI  */
#line 2882 "./CParse/parser.y"
                                                                                    {
		   (yyval.node) = 0;
		   if ((yyvsp[-5].tmap).method) {
		     (yyval.node) = new_node("typemapcopy");
		     Setattr((yyval.node),"method",(yyvsp[-5].tmap).method);
		     Setattr((yyval.node),"pattern", Getattr((yyvsp[-1].p),"pattern"));
		     appendChild((yyval.node),(yyvsp[-3].p));
		     Delete((yyvsp[-5].tmap).method);
		   }
		   Delete((yyvsp[-5].tmap).kwargs);
	       }
#line 6536 "CParse/parser.c"
    break;

  case 95: /* typemap_type: kwargs  */
#line 2897 "./CParse/parser.y"
                        {
		 String *name = Getattr((yyvsp[0].node), "name");
		 Hash *p = nextSibling((yyvsp[0].node));
		 (yyval.tmap).method = name;
		 (yyval.tmap).kwargs = p;
		 if (Getattr((yyvsp[0].node), "value")) {
		   Swig_error(cparse_file, cparse_line,
			      "%%typemap method shouldn't have a value specified.\n");
		 }
		 while (p) {
		   if (!Getattr(p, "value")) {
		     Swig_error(cparse_file, cparse_line,
				"%%typemap attribute '%s' is missing its value.  If this is specifying the target language, that's no longer supported: use #ifdef SWIG<LANG> instead.\n",
				Getattr(p, "name"));
		     /* Set to empty value to avoid segfaults later. */
		     Setattr(p, "value", NewStringEmpty());
		   }
		   p = nextSibling(p);
		 }
                }
#line 6561 "CParse/parser.c"
    break;

  case 96: /* tm_list: tm_list_builder  */
#line 2919 "./CParse/parser.y"
                                 {
		 (yyval.p) = (yyvsp[0].pbuilder).parms;
	       }
#line 6569 "CParse/parser.c"
    break;

  case 97: /* tm_list_builder: typemap_parm  */
#line 2924 "./CParse/parser.y"
                              {
                 (yyval.pbuilder).parms = (yyval.pbuilder).last = (yyvsp[0].p);
	       }
#line 6577 "CParse/parser.c"
    break;

  case 98: /* tm_list_builder: tm_list_builder COMMA typemap_parm  */
#line 2927 "./CParse/parser.y"
                                                        {
		 // Build a linked list in the order specified, but avoiding
		 // a right recursion rule because "Right recursion uses up
		 // space on the Bison stack in proportion to the number of
		 // elements in the sequence".
		 set_nextSibling((yyvsp[-2].pbuilder).last, (yyvsp[0].p));
		 (yyval.pbuilder).parms = (yyvsp[-2].pbuilder).parms;
		 (yyval.pbuilder).last = (yyvsp[0].p);
	       }
#line 6591 "CParse/parser.c"
    break;

  case 99: /* typemap_parm: type plain_declarator  */
#line 2938 "./CParse/parser.y"
                                       {
                  Parm *parm;
		  SwigType_push((yyvsp[-1].type),(yyvsp[0].decl).type);
		  (yyval.p) = new_node("typemapitem");
		  parm = NewParmWithoutFileLineInfo((yyvsp[-1].type),(yyvsp[0].decl).id);
		  Setattr((yyval.p),"pattern",parm);
		  Setattr((yyval.p),"parms", (yyvsp[0].decl).parms);
		  Delete(parm);
		  /*		  $$ = NewParmWithoutFileLineInfo($type,$plain_declarator.id);
				  Setattr($$,"parms",$plain_declarator.parms); */
                }
#line 6607 "CParse/parser.c"
    break;

  case 100: /* typemap_parm: LPAREN parms RPAREN  */
#line 2949 "./CParse/parser.y"
                                     {
                  (yyval.p) = new_node("typemapitem");
		  Setattr((yyval.p),"pattern",(yyvsp[-1].pl));
		  /*		  Setattr($$,"multitype",$parms); */
               }
#line 6617 "CParse/parser.c"
    break;

  case 101: /* typemap_parm: LPAREN parms RPAREN LPAREN parms RPAREN  */
#line 2954 "./CParse/parser.y"
                                                                      {
		 (yyval.p) = new_node("typemapitem");
		 Setattr((yyval.p),"pattern", (yyvsp[-4].pl));
		 /*                 Setattr($$,"multitype",$in); */
		 Setattr((yyval.p),"parms",(yyvsp[-1].pl));
               }
#line 6628 "CParse/parser.c"
    break;

  case 102: /* types_directive: TYPES LPAREN parms RPAREN stringbracesemi  */
#line 2967 "./CParse/parser.y"
                                                            {
                   (yyval.node) = new_node("types");
		   Setattr((yyval.node),"parms",(yyvsp[-2].pl));
                   if ((yyvsp[0].str))
		     Setattr((yyval.node),"convcode",NewString((yyvsp[0].str)));
               }
#line 6639 "CParse/parser.c"
    break;

  case 103: /* template_directive: SWIGTEMPLATE LPAREN idstringopt RPAREN idcolonnt LESSTHAN valparms GREATERTHAN SEMI  */
#line 2979 "./CParse/parser.y"
                                                                                                        {
                  Parm *p;
		  Node *n = 0;
		  Node *outer_class = currentOuterClass;
		  Symtab *tscope = 0;
		  String *symname = (yyvsp[-6].id) ? NewString((yyvsp[-6].id)) : 0;
		  int errored_flag = 0;
		  String *idcolonnt;

		  (yyval.node) = 0;

		  tscope = Swig_symbol_current();          /* Get the current scope */

		  /* If the class name is qualified, we need to create or lookup namespace entries */
		  idcolonnt = resolve_create_node_scope((yyvsp[-4].str), 0, &errored_flag);

		  if (!errored_flag) {
		    if (nscope_inner && Strcmp(nodeType(nscope_inner), "class") == 0)
		      outer_class = nscope_inner;

		    /*
		      We use the new namespace entry 'nscope' only to
		      emit the template node. The template parameters are
		      resolved in the current 'tscope'.

		      This is closer to the C++ (typedef) behavior.
		    */
		    n = Swig_cparse_template_locate(idcolonnt, (yyvsp[-2].p), symname, tscope);
		  }

		  /* Patch the argument types to respect namespaces */
		  p = (yyvsp[-2].p);
		  while (p) {
		    SwigType *value = Getattr(p,"value");
		    if (!value) {
		      SwigType *ty = Getattr(p,"type");
		      if (ty) {
			SwigType *rty = 0;
			int reduce = template_reduce;
			if (reduce || !SwigType_ispointer(ty)) {
			  rty = Swig_symbol_typedef_reduce(ty,tscope);
			  if (!reduce) reduce = SwigType_ispointer(rty);
			}
			ty = reduce ? Swig_symbol_type_qualify(rty,tscope) : Swig_symbol_type_qualify(ty,tscope);
			Setattr(p,"type",ty);
			Delete(ty);
			Delete(rty);
		      }
		    } else {
		      value = Swig_symbol_type_qualify(value,tscope);
		      Setattr(p,"value",value);
		      Delete(value);
		    }

		    p = nextSibling(p);
		  }

		  /* Look for the template */
		  {
                    Node *nn = n;
                    Node *linklistend = 0;
                    Node *linkliststart = 0;
                    while (nn) {
                      Node *templnode = 0;
                      if (GetFlag(nn, "instantiate")) {
			Delattr(nn, "instantiate");
			{
			  int nnisclass = (Strcmp(Getattr(nn, "templatetype"), "class") == 0); /* class template not a classforward nor function template */
			  Parm *tparms = Getattr(nn, "templateparms");
			  int specialized = !tparms; /* fully specialized (an explicit specialization) */
			  String *tname = Copy(idcolonnt);
			  Node *primary_template = Swig_symbol_clookup(tname, 0);

			  /* Expand the template */
			  ParmList *temparms = Swig_cparse_template_parms_expand((yyvsp[-2].p), primary_template, nn);

                          templnode = copy_node(nn);
			  update_nested_classes(templnode); /* update classes nested within template */
                          /* We need to set the node name based on name used to instantiate */
                          Setattr(templnode,"name",tname);
			  Delete(tname);
                          if (!specialized) {
                            Delattr(templnode,"sym:typename");
                          } else {
                            Setattr(templnode,"sym:typename","1");
                          }
			  /* for now, nested %template is allowed only in the same scope as the template declaration */
                          if (symname && !(nnisclass && ((outer_class && (outer_class != Getattr(nn, "nested:outer")))
			    ||(extendmode && current_class && (current_class != Getattr(nn, "nested:outer")))))) {
			    /*
			       Comment this out for 1.3.28. We need to
			       re-enable it later but first we need to
			       move %ignore from using %rename to use
			       %feature(ignore).

			       String *symname = Swig_name_make(templnode, 0, symname, 0, 0);
			    */
                            Swig_cparse_template_expand(templnode, symname, temparms, tscope);
                            Setattr(templnode, "sym:name", symname);
                          } else {
                            static int cnt = 0;
                            String *nname = NewStringf("__dummy_%d__", cnt++);
                            Swig_cparse_template_expand(templnode,nname,temparms,tscope);
                            Setattr(templnode,"sym:name",nname);
                            SetFlag(templnode,"hidden");
			    Delete(nname);
                            Setattr(templnode,"feature:onlychildren", "typemap,typemapitem,typemapcopy,typedef,types,fragment,apply");
			    if (symname) {
			      Swig_warning(WARN_PARSE_NESTED_TEMPLATE, cparse_file, cparse_line, "Named nested template instantiations not supported. Processing as if no name was given to %%template().\n");
			    }
                          }
                          Delattr(templnode,"templatetype");
                          Setattr(templnode,"template",nn);
                          Setfile(templnode,cparse_file);
                          Setline(templnode,cparse_line);
                          Delete(temparms);
			  if (outer_class && nnisclass) {
			    SetFlag(templnode, "nested");
			    Setattr(templnode, "nested:outer", outer_class);
			  }
                          add_symbols_copy(templnode);

			  if (Equal(nodeType(templnode), "classforward") && !(GetFlag(templnode, "feature:ignore") || GetFlag(templnode, "hidden"))) {
			    SWIG_WARN_NODE_BEGIN(templnode);
			    /* A full template class definition is required in order to wrap a template class as a proxy class so this %template is ineffective. */
			    if (GetFlag(templnode, "nested:forward"))
			      Swig_warning(WARN_PARSE_TEMPLATE_NESTED, cparse_file, cparse_line, "Unsupported template nested class '%s' cannot be used to instantiate a full template class with name '%s'.\n", Swig_name_decl(templnode), Getattr(templnode, "sym:name"));
			    else
			      Swig_warning(WARN_PARSE_TEMPLATE_FORWARD, cparse_file, cparse_line, "Template forward class '%s' cannot be used to instantiate a full template class with name '%s'.\n", Swig_name_decl(templnode), Getattr(templnode, "sym:name"));
			    SWIG_WARN_NODE_END(templnode);
			  }

                          if (Strcmp(nodeType(templnode),"class") == 0) {

                            /* Identify pure abstract methods */
                            Setattr(templnode,"abstracts", pure_abstracts(firstChild(templnode)));

                            /* Set up inheritance in symbol table */
                            {
                              Symtab  *csyms;
                              List *baselist = Getattr(templnode,"baselist");
                              csyms = Swig_symbol_current();
                              Swig_symbol_setscope(Getattr(templnode,"symtab"));
                              if (baselist) {
                                List *bases = Swig_make_inherit_list(Getattr(templnode,"name"),baselist, Namespaceprefix);
                                if (bases) {
                                  Iterator s;
                                  for (s = First(bases); s.item; s = Next(s)) {
                                    Symtab *st = Getattr(s.item,"symtab");
                                    if (st) {
				      Setfile(st,Getfile(s.item));
				      Setline(st,Getline(s.item));
                                      Swig_symbol_inherit(st);
                                    }
                                  }
				  Delete(bases);
                                }
                              }
                              Swig_symbol_setscope(csyms);
                            }

                            /* Merge in %extend methods for this class.
			       This only merges methods within %extend for a template specialized class such as
			         template<typename T> class K {}; %extend K<int> { ... }
			       The copy_node() call above has already added in the generic %extend methods such as
			         template<typename T> class K {}; %extend K { ... } */

			    /* !!! This may be broken.  We may have to add the
			       %extend methods at the beginning of the class */
                            {
                              String *stmp = 0;
                              String *clsname;
                              Node *am;
                              if (Namespaceprefix) {
                                clsname = stmp = NewStringf("%s::%s", Namespaceprefix, Getattr(templnode,"name"));
                              } else {
                                clsname = Getattr(templnode,"name");
                              }
                              am = Getattr(Swig_extend_hash(),clsname);
                              if (am) {
                                Symtab *st = Swig_symbol_current();
                                Swig_symbol_setscope(Getattr(templnode,"symtab"));
                                /*			    Printf(stdout,"%s: %s %p %p\n", Getattr(templnode,"name"), clsname, Swig_symbol_current(), Getattr(templnode,"symtab")); */
                                Swig_extend_merge(templnode,am);
                                Swig_symbol_setscope(st);
				Swig_extend_append_previous(templnode,am);
                                Delattr(Swig_extend_hash(),clsname);
                              }
			      if (stmp) Delete(stmp);
                            }

                            /* Add to classes hash */
			    if (!classes)
			      classes = NewHash();

			    if (Namespaceprefix) {
			      String *temp = NewStringf("%s::%s", Namespaceprefix, Getattr(templnode,"name"));
			      Setattr(classes,temp,templnode);
			      Delete(temp);
			    } else {
			      String *qs = Swig_symbol_qualifiedscopename(templnode);
			      Setattr(classes, qs,templnode);
			      Delete(qs);
			    }
                          }
                        }

                        /* all the overloaded function templates are added into a linked list */
                        if (!linkliststart)
                          linkliststart = templnode;
                        if (nscope_inner) {
                          /* non-global namespace */
                          if (templnode) {
                            appendChild(nscope_inner,templnode);
			    Delete(templnode);
                            if (nscope) (yyval.node) = nscope;
                          }
                        } else {
                          /* global namespace */
                          if (!linklistend) {
                            (yyval.node) = templnode;
                          } else {
                            set_nextSibling(linklistend,templnode);
			    Delete(templnode);
                          }
                          linklistend = templnode;
                        }
                      }
                      nn = Getattr(nn,"sym:nextSibling"); /* repeat for overloaded function templates. If a class template there will never be a sibling. */
                    }
                    update_defaultargs(linkliststart);
                    update_abstracts(linkliststart);
		  }
	          Swig_symbol_setscope(tscope);
		  Delete(Namespaceprefix);
		  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		  Delete(symname);
                }
#line 6882 "CParse/parser.c"
    break;

  case 104: /* warn_directive: WARN string  */
#line 3223 "./CParse/parser.y"
                             {
		  Swig_warning(0,cparse_file, cparse_line,"%s\n", (yyvsp[0].str));
		  (yyval.node) = 0;
               }
#line 6891 "CParse/parser.c"
    break;

  case 105: /* c_declaration: c_decl  */
#line 3233 "./CParse/parser.y"
                         {
                    (yyval.node) = (yyvsp[0].node); 
                    if ((yyval.node)) {
   		      add_symbols((yyval.node));
                      default_arguments((yyval.node));
   	            }
                }
#line 6903 "CParse/parser.c"
    break;

  case 108: /* $@3: %empty  */
#line 3245 "./CParse/parser.y"
                                       {
		  if (Strcmp((yyvsp[-1].str),"C") == 0) {
		    cparse_externc = 1;
		  }
		}
#line 6913 "CParse/parser.c"
    break;

  case 109: /* c_declaration: EXTERN string LBRACE $@3 interface RBRACE  */
#line 3249 "./CParse/parser.y"
                                   {
		  cparse_externc = 0;
		  if (Strcmp((yyvsp[-4].str),"C") == 0) {
		    Node *n = firstChild((yyvsp[-1].node));
		    (yyval.node) = new_node("extern");
		    Setattr((yyval.node),"name",(yyvsp[-4].str));
		    appendChild((yyval.node),n);
		    while (n) {
		      String *s = Getattr(n, "storage");
		      if (s) {
			if (Strstr(s, "thread_local")) {
			  Insert(s,0,"externc ");
			} else if (!Equal(s, "typedef")) {
			  Setattr(n,"storage","externc");
			}
		      } else {
			Setattr(n,"storage","externc");
		      }
		      n = nextSibling(n);
		    }
		  } else {
		    if (!Equal((yyvsp[-4].str),"C++")) {
		      Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\".\n", (yyvsp[-4].str));
		    }
		    (yyval.node) = new_node("extern");
		    Setattr((yyval.node),"name",(yyvsp[-4].str));
		    appendChild((yyval.node),firstChild((yyvsp[-1].node)));
		  }
                }
#line 6947 "CParse/parser.c"
    break;

  case 110: /* c_declaration: cpp_lambda_decl  */
#line 3278 "./CParse/parser.y"
                                  {
		  (yyval.node) = (yyvsp[0].node);
		  SWIG_WARN_NODE_BEGIN((yyval.node));
		  Swig_warning(WARN_CPP11_LAMBDA, cparse_file, cparse_line, "Lambda expressions and closures are not fully supported yet.\n");
		  SWIG_WARN_NODE_END((yyval.node));
		}
#line 6958 "CParse/parser.c"
    break;

  case 111: /* c_declaration: USING idcolon EQUAL type plain_declarator SEMI  */
#line 3284 "./CParse/parser.y"
                                                                 {
		  /* Convert using statement to a typedef statement */
		  (yyval.node) = new_node("cdecl");
		  Setattr((yyval.node),"type",(yyvsp[-2].type));
		  Setattr((yyval.node),"storage","typedef");
		  Setattr((yyval.node),"name",(yyvsp[-4].str));
		  Setattr((yyval.node),"decl",(yyvsp[-1].decl).type);
		  SetFlag((yyval.node),"typealias");
		  add_symbols((yyval.node));
		}
#line 6973 "CParse/parser.c"
    break;

  case 112: /* c_declaration: TEMPLATE LESSTHAN template_parms GREATERTHAN USING idcolon EQUAL type plain_declarator SEMI  */
#line 3294 "./CParse/parser.y"
                                                                                                              {
		  /* Convert alias template to a "template" typedef statement */
		  (yyval.node) = new_node("template");
		  Setattr((yyval.node),"type",(yyvsp[-2].type));
		  Setattr((yyval.node),"storage","typedef");
		  Setattr((yyval.node),"name",(yyvsp[-4].str));
		  Setattr((yyval.node),"decl",(yyvsp[-1].decl).type);
		  Setattr((yyval.node),"templateparms",(yyvsp[-7].tparms));
		  Setattr((yyval.node),"templatetype","cdecl");
		  SetFlag((yyval.node),"aliastemplate");
		  add_symbols((yyval.node));
		}
#line 6990 "CParse/parser.c"
    break;

  case 114: /* c_decl: storage_class type declarator cpp_const initializer c_decl_tail  */
#line 3313 "./CParse/parser.y"
                                                                          {
	      String *decl = (yyvsp[-3].decl).type;
              (yyval.node) = new_node("cdecl");
	      if ((yyvsp[-2].dtype).qualifier)
	        decl = add_qualifier_to_declarator((yyvsp[-3].decl).type, (yyvsp[-2].dtype).qualifier);
	      Setattr((yyval.node),"refqualifier",(yyvsp[-2].dtype).refqualifier);
	      Setattr((yyval.node),"type",(yyvsp[-4].type));
	      Setattr((yyval.node),"storage",(yyvsp[-5].str));
	      Setattr((yyval.node),"name",(yyvsp[-3].decl).id);
	      Setattr((yyval.node),"decl",decl);
	      Setattr((yyval.node),"parms",(yyvsp[-3].decl).parms);
	      Setattr((yyval.node),"value",(yyvsp[-1].dtype).val);
	      if ((yyvsp[-1].dtype).stringval) Setattr((yyval.node), "stringval", (yyvsp[-1].dtype).stringval);
	      if ((yyvsp[-1].dtype).numval) Setattr((yyval.node), "numval", (yyvsp[-1].dtype).numval);
	      Setattr((yyval.node),"throws",(yyvsp[-2].dtype).throws);
	      Setattr((yyval.node),"throw",(yyvsp[-2].dtype).throwf);
	      Setattr((yyval.node),"noexcept",(yyvsp[-2].dtype).nexcept);
	      Setattr((yyval.node),"final",(yyvsp[-2].dtype).final);
	      if ((yyvsp[-1].dtype).val && (yyvsp[-1].dtype).type) {
		/* store initializer type as it might be different to the declared type */
		SwigType *valuetype = NewSwigType((yyvsp[-1].dtype).type);
		if (Len(valuetype) > 0) {
		  Setattr((yyval.node), "valuetype", valuetype);
		} else {
		  /* If we can't determine the initializer type use the declared type. */
		  Setattr((yyval.node), "valuetype", (yyvsp[-4].type));
		}
		Delete(valuetype);
	      }
	      if (!(yyvsp[0].node)) {
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr((yyval.node),"code",code);
		  Delete(code);
		}
	      } else {
		Node *n = (yyvsp[0].node);
		/* Inherit attributes */
		while (n) {
		  String *type = Copy((yyvsp[-4].type));
		  Setattr(n,"type",type);
		  Setattr(n,"storage",(yyvsp[-5].str));
		  n = nextSibling(n);
		  Delete(type);
		}
	      }
	      if ((yyvsp[-1].dtype).bitfield) {
		Setattr((yyval.node),"bitfield", (yyvsp[-1].dtype).bitfield);
	      }

	      if ((yyvsp[-3].decl).id) {
		/* Ignore all scoped declarations, could be 1. out of class function definition 2. friend function declaration 3. ... */
		String *p = Swig_scopename_prefix((yyvsp[-3].decl).id);
		if (p) {
		  /* This is a special case. If the scope name of the declaration exactly
		     matches that of the declaration, then we will allow it. Otherwise, delete. */
		  if ((Namespaceprefix && Strcmp(p, Namespaceprefix) == 0) ||
		      (Classprefix && Strcmp(p, Classprefix) == 0)) {
		    String *lstr = Swig_scopename_last((yyvsp[-3].decl).id);
		    Setattr((yyval.node), "name", lstr);
		    Delete(lstr);
		    set_nextSibling((yyval.node), (yyvsp[0].node));
		  } else {
		    Delete((yyval.node));
		    (yyval.node) = (yyvsp[0].node);
		  }
		  Delete(p);
		} else if (Strncmp((yyvsp[-3].decl).id, "::", 2) == 0) {
		  /* global scope declaration/definition ignored */
		  Delete((yyval.node));
		  (yyval.node) = (yyvsp[0].node);
		} else {
		  set_nextSibling((yyval.node), (yyvsp[0].node));
		}
	      } else {
		Swig_error(cparse_file, cparse_line, "Missing symbol name for global declaration\n");
		(yyval.node) = 0;
	      }

	      if ((yyvsp[-2].dtype).qualifier && (yyvsp[-5].str) && Strstr((yyvsp[-5].str), "static"))
		Swig_error(cparse_file, cparse_line, "Static function %s cannot have a qualifier.\n", Swig_name_decl((yyval.node)));
	      Delete((yyvsp[-5].str));
           }
#line 7078 "CParse/parser.c"
    break;

  case 115: /* c_decl: storage_class type declarator cpp_const EQUAL error SEMI  */
#line 3396 "./CParse/parser.y"
                                                                      {
	      String *decl = (yyvsp[-4].decl).type;
	      (yyval.node) = new_node("cdecl");
	      if ((yyvsp[-3].dtype).qualifier)
	        decl = add_qualifier_to_declarator((yyvsp[-4].decl).type, (yyvsp[-3].dtype).qualifier);
	      Setattr((yyval.node), "refqualifier", (yyvsp[-3].dtype).refqualifier);
	      Setattr((yyval.node), "type", (yyvsp[-5].type));
	      Setattr((yyval.node), "storage", (yyvsp[-6].str));
	      Setattr((yyval.node), "name", (yyvsp[-4].decl).id);
	      Setattr((yyval.node), "decl", decl);
	      Setattr((yyval.node), "parms", (yyvsp[-4].decl).parms);

	      /* Set dummy value to avoid adding in code for handling missing value in later stages */
	      Setattr((yyval.node), "value", "*parse error*");
	      SetFlag((yyval.node), "valueignored");

	      Setattr((yyval.node), "throws", (yyvsp[-3].dtype).throws);
	      Setattr((yyval.node), "throw", (yyvsp[-3].dtype).throwf);
	      Setattr((yyval.node), "noexcept", (yyvsp[-3].dtype).nexcept);
	      Setattr((yyval.node), "final", (yyvsp[-3].dtype).final);

	      if ((yyvsp[-4].decl).id) {
		/* Ignore all scoped declarations, could be 1. out of class function definition 2. friend function declaration 3. ... */
		String *p = Swig_scopename_prefix((yyvsp[-4].decl).id);
		if (p) {
		  if ((Namespaceprefix && Strcmp(p, Namespaceprefix) == 0) ||
		      (Classprefix && Strcmp(p, Classprefix) == 0)) {
		    String *lstr = Swig_scopename_last((yyvsp[-4].decl).id);
		    Setattr((yyval.node), "name", lstr);
		    Delete(lstr);
		  } else {
		    Delete((yyval.node));
		    (yyval.node) = 0;
		  }
		  Delete(p);
		} else if (Strncmp((yyvsp[-4].decl).id, "::", 2) == 0) {
		  /* global scope declaration/definition ignored */
		  Delete((yyval.node));
		  (yyval.node) = 0;
		}
	      }

	      if ((yyvsp[-3].dtype).qualifier && (yyvsp[-6].str) && Strstr((yyvsp[-6].str), "static"))
		Swig_error(cparse_file, cparse_line, "Static function %s cannot have a qualifier.\n", Swig_name_decl((yyval.node)));
	      Delete((yyvsp[-6].str));
	   }
#line 7129 "CParse/parser.c"
    break;

  case 116: /* c_decl: storage_class AUTO declarator cpp_const ARROW cpp_alternate_rettype virt_specifier_seq_opt initializer c_decl_tail  */
#line 3444 "./CParse/parser.y"
                                                                                                                                {
              (yyval.node) = new_node("cdecl");
	      if ((yyvsp[-5].dtype).qualifier) SwigType_push((yyvsp[-6].decl).type, (yyvsp[-5].dtype).qualifier);
	      Setattr((yyval.node),"refqualifier",(yyvsp[-5].dtype).refqualifier);
	      Setattr((yyval.node),"type",(yyvsp[-3].type));
	      Setattr((yyval.node),"storage",(yyvsp[-8].str));
	      Setattr((yyval.node),"name",(yyvsp[-6].decl).id);
	      Setattr((yyval.node),"decl",(yyvsp[-6].decl).type);
	      Setattr((yyval.node),"parms",(yyvsp[-6].decl).parms);
	      Setattr((yyval.node),"throws",(yyvsp[-5].dtype).throws);
	      Setattr((yyval.node),"throw",(yyvsp[-5].dtype).throwf);
	      Setattr((yyval.node),"noexcept",(yyvsp[-5].dtype).nexcept);
	      Setattr((yyval.node),"final",(yyvsp[-5].dtype).final);
	      if (!(yyvsp[0].node)) {
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr((yyval.node),"code",code);
		  Delete(code);
		}
	      } else {
		Node *n = (yyvsp[0].node);
		while (n) {
		  String *type = Copy((yyvsp[-3].type));
		  Setattr(n,"type",type);
		  Setattr(n,"storage",(yyvsp[-8].str));
		  n = nextSibling(n);
		  Delete(type);
		}
	      }

	      if ((yyvsp[-6].decl).id) {
		/* Ignore all scoped declarations, could be 1. out of class function definition 2. friend function declaration 3. ... */
		String *p = Swig_scopename_prefix((yyvsp[-6].decl).id);
		if (p) {
		  if ((Namespaceprefix && Strcmp(p, Namespaceprefix) == 0) ||
		      (Classprefix && Strcmp(p, Classprefix) == 0)) {
		    String *lstr = Swig_scopename_last((yyvsp[-6].decl).id);
		    Setattr((yyval.node),"name",lstr);
		    Delete(lstr);
		    set_nextSibling((yyval.node), (yyvsp[0].node));
		  } else {
		    Delete((yyval.node));
		    (yyval.node) = (yyvsp[0].node);
		  }
		  Delete(p);
		} else if (Strncmp((yyvsp[-6].decl).id, "::", 2) == 0) {
		  /* global scope declaration/definition ignored */
		  Delete((yyval.node));
		  (yyval.node) = (yyvsp[0].node);
		}
	      } else {
		set_nextSibling((yyval.node), (yyvsp[0].node));
	      }

	      if ((yyvsp[-5].dtype).qualifier && (yyvsp[-8].str) && Strstr((yyvsp[-8].str), "static"))
		Swig_error(cparse_file, cparse_line, "Static function %s cannot have a qualifier.\n", Swig_name_decl((yyval.node)));
	      Delete((yyvsp[-8].str));
           }
#line 7192 "CParse/parser.c"
    break;

  case 117: /* c_decl: storage_class AUTO declarator cpp_const LBRACE  */
#line 3509 "./CParse/parser.y"
                                                            {
	      if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);

              (yyval.node) = new_node("cdecl");
	      if ((yyvsp[-1].dtype).qualifier) SwigType_push((yyvsp[-2].decl).type, (yyvsp[-1].dtype).qualifier);
	      Setattr((yyval.node), "refqualifier", (yyvsp[-1].dtype).refqualifier);
	      Setattr((yyval.node), "type", NewString("auto"));
	      Setattr((yyval.node), "storage", (yyvsp[-4].str));
	      Setattr((yyval.node), "name", (yyvsp[-2].decl).id);
	      Setattr((yyval.node), "decl", (yyvsp[-2].decl).type);
	      Setattr((yyval.node), "parms", (yyvsp[-2].decl).parms);
	      Setattr((yyval.node), "throws", (yyvsp[-1].dtype).throws);
	      Setattr((yyval.node), "throw", (yyvsp[-1].dtype).throwf);
	      Setattr((yyval.node), "noexcept", (yyvsp[-1].dtype).nexcept);
	      Setattr((yyval.node), "final", (yyvsp[-1].dtype).final);

	      if ((yyvsp[-2].decl).id) {
		/* Ignore all scoped declarations, could be 1. out of class function definition 2. friend function declaration 3. ... */
		String *p = Swig_scopename_prefix((yyvsp[-2].decl).id);
		if (p) {
		  if ((Namespaceprefix && Strcmp(p, Namespaceprefix) == 0) ||
		      (Classprefix && Strcmp(p, Classprefix) == 0)) {
		    String *lstr = Swig_scopename_last((yyvsp[-2].decl).id);
		    Setattr((yyval.node), "name", lstr);
		    Delete(lstr);
		  } else {
		    Delete((yyval.node));
		    (yyval.node) = 0;
		  }
		  Delete(p);
		} else if (Strncmp((yyvsp[-2].decl).id, "::", 2) == 0) {
		  /* global scope declaration/definition ignored */
		  Delete((yyval.node));
		  (yyval.node) = 0;
		}
	      }

	      if ((yyvsp[-1].dtype).qualifier && (yyvsp[-4].str) && Strstr((yyvsp[-4].str), "static"))
		Swig_error(cparse_file, cparse_line, "Static function %s cannot have a qualifier.\n", Swig_name_decl((yyval.node)));
	      Delete((yyvsp[-4].str));
	   }
#line 7238 "CParse/parser.c"
    break;

  case 118: /* c_decl: storage_class AUTO idcolon EQUAL definetype SEMI  */
#line 3551 "./CParse/parser.y"
                                                              {
	      SwigType *type = deduce_type(&(yyvsp[-1].dtype));
	      if (!type)
		type = NewString("auto");
	      (yyval.node) = new_node("cdecl");
	      Setattr((yyval.node), "type", type);
	      Setattr((yyval.node), "storage", (yyvsp[-5].str));
	      Setattr((yyval.node), "name", (yyvsp[-3].str));
	      Setattr((yyval.node), "decl", NewStringEmpty());
	      Setattr((yyval.node), "value", (yyvsp[-1].dtype).val);
	      if ((yyvsp[-1].dtype).stringval) Setattr((yyval.node), "stringval", (yyvsp[-1].dtype).stringval);
	      if ((yyvsp[-1].dtype).numval) Setattr((yyval.node), "numval", (yyvsp[-1].dtype).numval);
	      Setattr((yyval.node), "valuetype", type);
	      Delete((yyvsp[-5].str));
	      Delete(type);
	   }
#line 7259 "CParse/parser.c"
    break;

  case 119: /* c_decl: storage_class AUTO idcolon EQUAL error SEMI  */
#line 3568 "./CParse/parser.y"
                                                         {
	      SwigType *type = NewString("auto");
	      (yyval.node) = new_node("cdecl");
	      Setattr((yyval.node), "type", type);
	      Setattr((yyval.node), "storage", (yyvsp[-5].str));
	      Setattr((yyval.node), "name", (yyvsp[-3].str));
	      Setattr((yyval.node), "decl", NewStringEmpty());
	      Setattr((yyval.node), "valuetype", type);
	      Delete((yyvsp[-5].str));
	      Delete(type);
	   }
#line 7275 "CParse/parser.c"
    break;

  case 120: /* c_decl_tail: SEMI  */
#line 3583 "./CParse/parser.y"
                      { 
                   (yyval.node) = 0;
                   Clear(scanner_ccode); 
               }
#line 7284 "CParse/parser.c"
    break;

  case 121: /* c_decl_tail: COMMA declarator cpp_const initializer c_decl_tail  */
#line 3587 "./CParse/parser.y"
                                                                        {
		 (yyval.node) = new_node("cdecl");
		 if ((yyvsp[-2].dtype).qualifier) SwigType_push((yyvsp[-3].decl).type,(yyvsp[-2].dtype).qualifier);
		 Setattr((yyval.node),"refqualifier",(yyvsp[-2].dtype).refqualifier);
		 Setattr((yyval.node),"name",(yyvsp[-3].decl).id);
		 Setattr((yyval.node),"decl",(yyvsp[-3].decl).type);
		 Setattr((yyval.node),"parms",(yyvsp[-3].decl).parms);
		 Setattr((yyval.node),"value",(yyvsp[-1].dtype).val);
		 if ((yyvsp[-1].dtype).stringval) Setattr((yyval.node), "stringval", (yyvsp[-1].dtype).stringval);
		 if ((yyvsp[-1].dtype).numval) Setattr((yyval.node), "numval", (yyvsp[-1].dtype).numval);
		 Setattr((yyval.node),"throws",(yyvsp[-2].dtype).throws);
		 Setattr((yyval.node),"throw",(yyvsp[-2].dtype).throwf);
		 Setattr((yyval.node),"noexcept",(yyvsp[-2].dtype).nexcept);
		 Setattr((yyval.node),"final",(yyvsp[-2].dtype).final);
		 if ((yyvsp[-1].dtype).bitfield) {
		   Setattr((yyval.node),"bitfield", (yyvsp[-1].dtype).bitfield);
		 }
		 if (!(yyvsp[0].node)) {
		   if (Len(scanner_ccode)) {
		     String *code = Copy(scanner_ccode);
		     Setattr((yyval.node),"code",code);
		     Delete(code);
		   }
		 } else {
		   set_nextSibling((yyval.node), (yyvsp[0].node));
		 }
	       }
#line 7316 "CParse/parser.c"
    break;

  case 122: /* c_decl_tail: LBRACE  */
#line 3614 "./CParse/parser.y"
                        { 
                   if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
                   (yyval.node) = 0;
               }
#line 7325 "CParse/parser.c"
    break;

  case 123: /* c_decl_tail: error  */
#line 3618 "./CParse/parser.y"
                       {
		   (yyval.node) = 0;
		   if (yychar == RPAREN) {
		       Swig_error(cparse_file, cparse_line, "Unexpected closing parenthesis (')').\n");
		   } else {
		       Swig_error(cparse_file, cparse_line, "Syntax error - possibly a missing semicolon (';').\n");
		   }
		   Exit(EXIT_FAILURE);
               }
#line 7339 "CParse/parser.c"
    break;

  case 125: /* initializer: COLON expr  */
#line 3630 "./CParse/parser.y"
                           {
		(yyval.dtype) = default_dtype;
		(yyval.dtype).bitfield = (yyvsp[0].dtype).val;
	      }
#line 7348 "CParse/parser.c"
    break;

  case 129: /* cpp_alternate_rettype: c_enum_key idcolon  */
#line 3639 "./CParse/parser.y"
                                   {
		(yyval.type) = (yyvsp[0].str);
		Insert((yyval.type), 0, "enum ");
	      }
#line 7357 "CParse/parser.c"
    break;

  case 131: /* cpp_alternate_rettype: idcolon  */
#line 3644 "./CParse/parser.y"
                        { (yyval.type) = (yyvsp[0].str); }
#line 7363 "CParse/parser.c"
    break;

  case 132: /* cpp_alternate_rettype: idcolon AND  */
#line 3645 "./CParse/parser.y"
                            {
                (yyval.type) = (yyvsp[-1].str);
                SwigType_add_reference((yyval.type));
              }
#line 7372 "CParse/parser.c"
    break;

  case 133: /* cpp_alternate_rettype: idcolon LAND  */
#line 3649 "./CParse/parser.y"
                             {
                (yyval.type) = (yyvsp[-1].str);
                SwigType_add_rvalue_reference((yyval.type));
              }
#line 7381 "CParse/parser.c"
    break;

  case 134: /* cpp_alternate_rettype: CONST_QUAL idcolon AND  */
#line 3653 "./CParse/parser.y"
                                       {
                (yyval.type) = (yyvsp[-1].str);
                SwigType_add_qualifier((yyval.type), "const");
                SwigType_add_reference((yyval.type));
              }
#line 7391 "CParse/parser.c"
    break;

  case 135: /* cpp_alternate_rettype: CONST_QUAL idcolon LAND  */
#line 3658 "./CParse/parser.y"
                                        {
                (yyval.type) = (yyvsp[-1].str);
                SwigType_add_qualifier((yyval.type), "const");
                SwigType_add_rvalue_reference((yyval.type));
              }
#line 7401 "CParse/parser.c"
    break;

  case 137: /* cpp_lambda_decl: storage_class AUTO idcolon EQUAL lambda_introducer lambda_template LPAREN parms RPAREN cpp_const lambda_body lambda_tail  */
#line 3674 "./CParse/parser.y"
                                                                                                                                           {
		  (yyval.node) = new_node("lambda");
		  Setattr((yyval.node),"name",(yyvsp[-9].str));
		  Delete((yyvsp[-11].str));
		  add_symbols((yyval.node));
	        }
#line 7412 "CParse/parser.c"
    break;

  case 138: /* cpp_lambda_decl: storage_class AUTO idcolon EQUAL lambda_introducer lambda_template LPAREN parms RPAREN cpp_const ARROW type lambda_body lambda_tail  */
#line 3680 "./CParse/parser.y"
                                                                                                                                                      {
		  (yyval.node) = new_node("lambda");
		  Setattr((yyval.node),"name",(yyvsp[-11].str));
		  Delete((yyvsp[-13].str));
		  add_symbols((yyval.node));
		}
#line 7423 "CParse/parser.c"
    break;

  case 139: /* cpp_lambda_decl: storage_class AUTO idcolon EQUAL lambda_introducer lambda_template lambda_body lambda_tail  */
#line 3686 "./CParse/parser.y"
                                                                                                             {
		  (yyval.node) = new_node("lambda");
		  Setattr((yyval.node),"name",(yyvsp[-5].str));
		  Delete((yyvsp[-7].str));
		  add_symbols((yyval.node));
		}
#line 7434 "CParse/parser.c"
    break;

  case 140: /* lambda_introducer: LBRACKET  */
#line 3694 "./CParse/parser.y"
                             {
		  if (skip_balanced('[',']') < 0) Exit(EXIT_FAILURE);
	        }
#line 7442 "CParse/parser.c"
    break;

  case 141: /* lambda_template: LESSTHAN  */
#line 3699 "./CParse/parser.y"
                           {
		  if (skip_balanced('<','>') < 0) Exit(EXIT_FAILURE);
		}
#line 7450 "CParse/parser.c"
    break;

  case 143: /* lambda_body: LBRACE  */
#line 3705 "./CParse/parser.y"
                     {
		  if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		}
#line 7458 "CParse/parser.c"
    break;

  case 145: /* $@4: %empty  */
#line 3710 "./CParse/parser.y"
                         {
		  if (skip_balanced('(',')') < 0) Exit(EXIT_FAILURE);
		}
#line 7466 "CParse/parser.c"
    break;

  case 146: /* lambda_tail: LPAREN $@4 SEMI  */
#line 3712 "./CParse/parser.y"
                       {
		}
#line 7473 "CParse/parser.c"
    break;

  case 147: /* c_enum_key: ENUM  */
#line 3722 "./CParse/parser.y"
                  {
		   (yyval.id) = "enum";
	      }
#line 7481 "CParse/parser.c"
    break;

  case 148: /* c_enum_key: ENUM CLASS  */
#line 3725 "./CParse/parser.y"
                           {
		   (yyval.id) = "enum class";
	      }
#line 7489 "CParse/parser.c"
    break;

  case 149: /* c_enum_key: ENUM STRUCT  */
#line 3728 "./CParse/parser.y"
                            {
		   (yyval.id) = "enum struct";
	      }
#line 7497 "CParse/parser.c"
    break;

  case 150: /* c_enum_inherit: COLON type_right  */
#line 3737 "./CParse/parser.y"
                                  {
                   (yyval.type) = (yyvsp[0].type);
              }
#line 7505 "CParse/parser.c"
    break;

  case 151: /* c_enum_inherit: %empty  */
#line 3740 "./CParse/parser.y"
                       { (yyval.type) = 0; }
#line 7511 "CParse/parser.c"
    break;

  case 152: /* c_enum_forward_decl: storage_class c_enum_key ename c_enum_inherit SEMI  */
#line 3747 "./CParse/parser.y"
                                                                         {
		   SwigType *ty = 0;
		   int scopedenum = (yyvsp[-2].id) && !Equal((yyvsp[-3].id), "enum");
		   (yyval.node) = new_node("enumforward");
		   ty = NewStringf("enum %s", (yyvsp[-2].id));
		   Setattr((yyval.node),"enumkey",(yyvsp[-3].id));
		   if (scopedenum)
		     SetFlag((yyval.node), "scopedenum");
		   Setattr((yyval.node),"name",(yyvsp[-2].id));
		   Setattr((yyval.node), "enumbase", (yyvsp[-1].type));
		   Setattr((yyval.node),"type",ty);
		   Setattr((yyval.node),"sym:weak", "1");
		   Delete((yyvsp[-4].str));
		   add_symbols((yyval.node));
	      }
#line 7531 "CParse/parser.c"
    break;

  case 153: /* c_enum_decl: storage_class c_enum_key ename c_enum_inherit LBRACE enumlist RBRACE SEMI  */
#line 3770 "./CParse/parser.y"
                                                                                         {
		  SwigType *ty = 0;
		  int scopedenum = (yyvsp[-5].id) && !Equal((yyvsp[-6].id), "enum");
		  (yyval.node) = new_enum_node((yyvsp[-4].type));
		  ty = NewStringf("enum %s", (yyvsp[-5].id));
		  Setattr((yyval.node),"enumkey",(yyvsp[-6].id));
		  if (scopedenum)
		    SetFlag((yyval.node), "scopedenum");
		  Setattr((yyval.node),"name",(yyvsp[-5].id));
		  Setattr((yyval.node),"type",ty);
		  appendChild((yyval.node),(yyvsp[-2].node));
		  add_symbols((yyval.node));      /* Add to tag space */

		  if (scopedenum) {
		    Swig_symbol_newscope();
		    Swig_symbol_setscopename((yyvsp[-5].id));
		    Delete(Namespaceprefix);
		    Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		  }

		  add_symbols((yyvsp[-2].node));      /* Add enum values to appropriate enum or enum class scope */

		  if (scopedenum) {
		    Setattr((yyval.node),"symtab", Swig_symbol_popscope());
		    Delete(Namespaceprefix);
		    Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		  }
		  Delete((yyvsp[-7].str));
               }
#line 7565 "CParse/parser.c"
    break;

  case 154: /* c_enum_decl: storage_class c_enum_key ename c_enum_inherit LBRACE enumlist RBRACE declarator cpp_const initializer c_decl_tail  */
#line 3799 "./CParse/parser.y"
                                                                                                                                   {
		 Node *n;
		 SwigType *ty = 0;
		 String   *unnamed = 0;
		 int       unnamedinstance = 0;
		 int scopedenum = (yyvsp[-8].id) && !Equal((yyvsp[-9].id), "enum");

		 (yyval.node) = new_enum_node((yyvsp[-7].type));
		 Setattr((yyval.node),"enumkey",(yyvsp[-9].id));
		 if (scopedenum)
		   SetFlag((yyval.node), "scopedenum");
		 if ((yyvsp[-8].id)) {
		   Setattr((yyval.node),"name",(yyvsp[-8].id));
		   ty = NewStringf("enum %s", (yyvsp[-8].id));
		 } else if ((yyvsp[-3].decl).id) {
		   unnamed = make_unnamed();
		   ty = NewStringf("enum %s", unnamed);
		   Setattr((yyval.node),"unnamed",unnamed);
                   /* name is not set for unnamed enum instances, e.g. enum { foo } Instance; */
		   if ((yyvsp[-10].str) && Cmp((yyvsp[-10].str),"typedef") == 0) {
		     Setattr((yyval.node),"name",(yyvsp[-3].decl).id);
                   } else {
                     unnamedinstance = 1;
                   }
		   Setattr((yyval.node),"storage",(yyvsp[-10].str));
		 }
		 if ((yyvsp[-3].decl).id && Cmp((yyvsp[-10].str),"typedef") == 0) {
		   Setattr((yyval.node),"tdname",(yyvsp[-3].decl).id);
                   Setattr((yyval.node),"allows_typedef","1");
                 }
		 appendChild((yyval.node),(yyvsp[-5].node));
		 n = new_node("cdecl");
		 Setattr(n,"type",ty);
		 Setattr(n,"name",(yyvsp[-3].decl).id);
		 Setattr(n,"storage",(yyvsp[-10].str));
		 Setattr(n,"decl",(yyvsp[-3].decl).type);
		 Setattr(n,"parms",(yyvsp[-3].decl).parms);
		 Setattr(n,"unnamed",unnamed);

                 if (unnamedinstance) {
		   SwigType *cty = NewString("enum ");
		   Setattr((yyval.node),"type",cty);
		   SetFlag((yyval.node),"unnamedinstance");
		   SetFlag(n,"unnamedinstance");
		   Delete(cty);
                 }
		 if ((yyvsp[0].node)) {
		   Node *p = (yyvsp[0].node);
		   set_nextSibling(n,p);
		   while (p) {
		     SwigType *cty = Copy(ty);
		     Setattr(p,"type",cty);
		     Setattr(p,"unnamed",unnamed);
		     Setattr(p,"storage",(yyvsp[-10].str));
		     Delete(cty);
		     p = nextSibling(p);
		   }
		 } else {
		   if (Len(scanner_ccode)) {
		     String *code = Copy(scanner_ccode);
		     Setattr(n,"code",code);
		     Delete(code);
		   }
		 }

                 /* Ensure that typedef enum ABC {foo} XYZ; uses XYZ for sym:name, like structs.
                  * Note that class_rename/yyrename are bit of a mess so used this simple approach to change the name. */
                 if ((yyvsp[-3].decl).id && (yyvsp[-8].id) && Cmp((yyvsp[-10].str),"typedef") == 0) {
		   String *name = NewString((yyvsp[-3].decl).id);
                   Setattr((yyval.node), "parser:makename", name);
		   Delete(name);
                 }

		 add_symbols((yyval.node));       /* Add enum to tag space */
		 set_nextSibling((yyval.node),n);
		 Delete(n);

		 if (scopedenum) {
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename((yyvsp[-8].id));
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		 }

		 add_symbols((yyvsp[-5].node));      /* Add enum values to appropriate enum or enum class scope */

		 if (scopedenum) {
		   Setattr((yyval.node),"symtab", Swig_symbol_popscope());
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		 }

	         add_symbols(n);
		 Delete((yyvsp[-10].str));
		 Delete(unnamed);
	       }
#line 7666 "CParse/parser.c"
    break;

  case 155: /* c_constructor_decl: storage_class type LPAREN parms RPAREN ctor_end  */
#line 3897 "./CParse/parser.y"
                                                                     {
                   /* This is a sick hack.  If the ctor_end has parameters,
                      and the parms parameter only has 1 parameter, this
                      could be a declaration of the form:

                         type (id)(parms)

			 Otherwise it's an error. */
                    int err = 0;
                    (yyval.node) = 0;

		    if ((ParmList_len((yyvsp[-2].pl)) == 1) && (!Swig_scopename_check((yyvsp[-4].type)))) {
		      SwigType *ty = Getattr((yyvsp[-2].pl),"type");
		      String *name = Getattr((yyvsp[-2].pl),"name");
		      err = 1;
		      if (!name) {
			(yyval.node) = new_node("cdecl");
			Setattr((yyval.node),"type",(yyvsp[-4].type));
			Setattr((yyval.node),"storage",(yyvsp[-5].str));
			Setattr((yyval.node),"name",ty);

			if ((yyvsp[0].decl).have_parms) {
			  SwigType *decl = NewStringEmpty();
			  SwigType_add_function(decl,(yyvsp[0].decl).parms);
			  Setattr((yyval.node),"decl",decl);
			  Setattr((yyval.node),"parms",(yyvsp[0].decl).parms);
			  if (Len(scanner_ccode)) {
			    String *code = Copy(scanner_ccode);
			    Setattr((yyval.node),"code",code);
			    Delete(code);
			  }
			}
			if ((yyvsp[0].decl).defarg)
			  Setattr((yyval.node), "value", (yyvsp[0].decl).defarg);
			if ((yyvsp[0].decl).stringdefarg)
			  Setattr((yyval.node), "stringval", (yyvsp[0].decl).stringdefarg);
			if ((yyvsp[0].decl).numdefarg)
			  Setattr((yyval.node), "numval", (yyvsp[0].decl).numdefarg);
			Setattr((yyval.node),"throws",(yyvsp[0].decl).throws);
			Setattr((yyval.node),"throw",(yyvsp[0].decl).throwf);
			Setattr((yyval.node),"noexcept",(yyvsp[0].decl).nexcept);
			Setattr((yyval.node),"final",(yyvsp[0].decl).final);
			err = 0;
		      }
		    }
		    Delete((yyvsp[-5].str));
		    if (err) {
		      Swig_error(cparse_file,cparse_line,"Syntax error in input(2).\n");
		      Exit(EXIT_FAILURE);
		    }
                }
#line 7722 "CParse/parser.c"
    break;

  case 162: /* @5: %empty  */
#line 3968 "./CParse/parser.y"
                                                                                            {
                   String *prefix;
                   List *bases = 0;
		   Node *scope = 0;
		   int errored_flag = 0;
		   String *code;
		   (yyval.node) = new_node("class");
		   Setattr((yyval.node),"kind",(yyvsp[-4].type));
		   if ((yyvsp[-1].bases)) {
		     Setattr((yyval.node),"baselist", Getattr((yyvsp[-1].bases),"public"));
		     Setattr((yyval.node),"protectedbaselist", Getattr((yyvsp[-1].bases),"protected"));
		     Setattr((yyval.node),"privatebaselist", Getattr((yyvsp[-1].bases),"private"));
		   }
		   Setattr((yyval.node),"allows_typedef","1");

		   /* Temporary unofficial symtab for use until add_symbols() adds "sym:symtab" */
		   Setattr((yyval.node), "unofficial:symtab", Swig_symbol_current());
		  
		   /* If the class name is qualified.  We need to create or lookup namespace/scope entries */
		   scope = resolve_create_node_scope((yyvsp[-3].str), 1, &errored_flag);
		   /* save nscope_inner to the class - it may be overwritten in nested classes*/
		   Setattr((yyval.node), "nested:innerscope", nscope_inner);
		   Setattr((yyval.node), "nested:nscope", nscope);
		   Setfile(scope,cparse_file);
		   Setline(scope,cparse_line);
		   Setattr((yyval.node), "name", scope);

		   if (currentOuterClass) {
		     SetFlag((yyval.node), "nested");
		     Setattr((yyval.node), "nested:outer", currentOuterClass);
		     set_access_mode((yyval.node));
		   }
		   Swig_features_get(Swig_cparse_features(), Namespaceprefix, Getattr((yyval.node), "name"), 0, (yyval.node));
		   /* save yyrename to the class attribute, to be used later in add_symbols()*/
		   Setattr((yyval.node), "class_rename", make_name((yyval.node), scope, 0));
		   Setattr((yyval.node), "Classprefix", scope);
		   Classprefix = NewString(scope);
		   /* Deal with inheritance  */
		   if ((yyvsp[-1].bases))
		     bases = Swig_make_inherit_list(scope, Getattr((yyvsp[-1].bases), "public"), Namespaceprefix);
		   prefix = SwigType_istemplate_templateprefix(scope);
		   if (prefix) {
		     String *fbase, *tbase;
		     if (Namespaceprefix) {
		       fbase = NewStringf("%s::%s", Namespaceprefix, scope);
		       tbase = NewStringf("%s::%s", Namespaceprefix, prefix);
		     } else {
		       fbase = Copy(scope);
		       tbase = Copy(prefix);
		     }
		     Swig_name_inherit(tbase,fbase);
		     Delete(fbase);
		     Delete(tbase);
		   }
                   if (Strcmp((yyvsp[-4].type), "class") == 0) {
		     cplus_mode = CPLUS_PRIVATE;
		   } else {
		     cplus_mode = CPLUS_PUBLIC;
		   }
		   if (!cparse_cplusplus) {
		     set_scope_to_global();
		   }
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename(scope);
		   Swig_inherit_base_symbols(bases);
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   cparse_start_line = cparse_line;

		   /* If there are active template parameters, we need to make sure they are
                      placed in the class symbol table so we can catch shadows */

		   if (template_parameters) {
		     Parm *tp = template_parameters;
		     while(tp) {
		       String *tpname = Copy(Getattr(tp,"name"));
		       Node *tn = new_node("templateparm");
		       Setattr(tn,"name",tpname);
		       Swig_symbol_cadd(tpname,tn);
		       tp = nextSibling(tp);
		       Delete(tpname);
		     }
		   }
		   Delete(prefix);
		   inclass = 1;
		   currentOuterClass = (yyval.node);
		   if (cparse_cplusplusout) {
		     /* save the structure declaration to declare it in global scope for C++ to see */
		     code = get_raw_text_balanced('{', '}');
		     Setattr((yyval.node), "code", code);
		     Delete(code);
		   }
               }
#line 7820 "CParse/parser.c"
    break;

  case 163: /* cpp_class_decl: storage_class cpptype idcolon class_virt_specifier_opt inherit LBRACE @5 cpp_members RBRACE cpp_opt_declarators  */
#line 4060 "./CParse/parser.y"
                                                              {
		   Node *p;
		   SwigType *ty;
		   Symtab *cscope;
		   Node *am = 0;
		   String *scpname = 0;
		   (yyval.node) = currentOuterClass;
		   currentOuterClass = Getattr((yyval.node), "nested:outer");
		   nscope_inner = Getattr((yyval.node), "nested:innerscope");
		   nscope = Getattr((yyval.node), "nested:nscope");
		   Delattr((yyval.node), "nested:innerscope");
		   Delattr((yyval.node), "nested:nscope");
		   if (nscope_inner && Strcmp(nodeType(nscope_inner), "class") == 0) { /* actual parent class for this class */
		     Node* forward_declaration = Swig_symbol_clookup_no_inherit(Getattr((yyval.node),"name"), Getattr(nscope_inner, "symtab"));
		     if (forward_declaration) {
		       Setattr((yyval.node), "access", Getattr(forward_declaration, "access"));
		     }
		     Setattr((yyval.node), "nested:outer", nscope_inner);
		     SetFlag((yyval.node), "nested");
                   }
		   if (!currentOuterClass)
		     inclass = 0;
		   cscope = Getattr((yyval.node), "unofficial:symtab");
		   Delattr((yyval.node), "unofficial:symtab");
		   
		   /* Check for pure-abstract class */
		   Setattr((yyval.node),"abstracts", pure_abstracts((yyvsp[-2].node)));
		   
		   /* This bit of code merges in a previously defined %extend directive (if any) */
		   {
		     String *clsname = Swig_symbol_qualifiedscopename(0);
		     am = Getattr(Swig_extend_hash(), clsname);
		     if (am) {
		       Swig_extend_merge((yyval.node), am);
		       Delattr(Swig_extend_hash(), clsname);
		     }
		     Delete(clsname);
		   }
		   if (!classes) classes = NewHash();
		   scpname = Swig_symbol_qualifiedscopename(0);
		   Setattr(classes, scpname, (yyval.node));

		   appendChild((yyval.node), (yyvsp[-2].node));
		   
		   if (am) 
		     Swig_extend_append_previous((yyval.node), am);

		   p = (yyvsp[0].node);
		   if (p && !nscope_inner) {
		     if (!cparse_cplusplus && currentOuterClass)
		       appendChild(currentOuterClass, p);
		     else
		      appendSibling((yyval.node), p);
		   }
		   
		   if (nscope_inner) {
		     ty = NewString(scpname); /* if the class is declared out of scope, let the declarator use fully qualified type*/
		   } else if (cparse_cplusplus && !cparse_externc) {
		     ty = NewString(Getattr((yyvsp[-3].node), "name"));
		   } else {
		     ty = NewStringf("%s %s", (yyvsp[-8].type), Getattr((yyvsp[-3].node), "name"));
		   }
		   while (p) {
		     Setattr(p, "storage", (yyvsp[-9].str));
		     Setattr(p, "type" ,ty);
		     if (!cparse_cplusplus && currentOuterClass && (!Getattr(currentOuterClass, "name"))) {
		       SetFlag(p, "hasconsttype");
		     }
		     p = nextSibling(p);
		   }
		   if ((yyvsp[0].node) && Cmp((yyvsp[-9].str),"typedef") == 0)
		     add_typedef_name((yyval.node), (yyvsp[0].node), Getattr((yyvsp[-3].node), "name"), cscope, scpname);
		   Delete(scpname);

		   if (cplus_mode != CPLUS_PUBLIC) {
		   /* we 'open' the class at the end, to allow %template
		      to add new members */
		     Node *pa = new_node("access");
		     Setattr(pa, "kind", "public");
		     cplus_mode = CPLUS_PUBLIC;
		     appendChild((yyval.node), pa);
		     Delete(pa);
		   }
		   if (currentOuterClass)
		     restore_access_mode((yyval.node));
		   Setattr((yyval.node), "symtab", Swig_symbol_popscope());
		   Classprefix = Getattr((yyval.node), "Classprefix");
		   Delattr((yyval.node), "Classprefix");
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   if (cplus_mode == CPLUS_PRIVATE) {
		     (yyval.node) = 0; /* skip private nested classes */
		   } else if (cparse_cplusplus && currentOuterClass && ignore_nested_classes && !GetFlag((yyval.node), "feature:flatnested")) {
		     (yyval.node) = nested_forward_declaration((yyvsp[-9].str), (yyvsp[-8].type), Getattr((yyvsp[-3].node), "name"), Copy(Getattr((yyvsp[-3].node), "name")), (yyvsp[0].node));
		   } else if (nscope_inner) {
		     /* this is tricky */
		     /* we add the declaration in the original namespace */
		     if (Strcmp(nodeType(nscope_inner), "class") == 0 && cparse_cplusplus && ignore_nested_classes && !GetFlag((yyval.node), "feature:flatnested"))
		       (yyval.node) = nested_forward_declaration((yyvsp[-9].str), (yyvsp[-8].type), Getattr((yyvsp[-3].node), "name"), Copy(Getattr((yyvsp[-3].node), "name")), (yyvsp[0].node));
		     appendChild(nscope_inner, (yyval.node));
		     Swig_symbol_setscope(Getattr(nscope_inner, "symtab"));
		     Delete(Namespaceprefix);
		     Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		     yyrename = Copy(Getattr((yyval.node), "class_rename"));
		     add_symbols((yyval.node));
		     Delattr((yyval.node), "class_rename");
		     /* but the variable definition in the current scope */
		     Swig_symbol_setscope(cscope);
		     Delete(Namespaceprefix);
		     Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		     add_symbols((yyvsp[0].node));
		     if (nscope) {
		       (yyval.node) = nscope; /* here we return recreated namespace tower instead of the class itself */
		       if ((yyvsp[0].node)) {
			 appendSibling((yyval.node), (yyvsp[0].node));
		       }
		     } else if (!SwigType_istemplate(ty) && template_parameters == 0) { /* for template we need the class itself */
		       (yyval.node) = (yyvsp[0].node);
		     }
		   } else {
		     Delete(yyrename);
		     yyrename = 0;
		     if (!cparse_cplusplus && currentOuterClass) { /* nested C structs go into global scope*/
		       Node *outer = currentOuterClass;
		       while (Getattr(outer, "nested:outer"))
			 outer = Getattr(outer, "nested:outer");
		       appendSibling(outer, (yyval.node));
		       Swig_symbol_setscope(cscope); /* declaration goes in the parent scope */
		       add_symbols((yyvsp[0].node));
		       set_scope_to_global();
		       Delete(Namespaceprefix);
		       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		       yyrename = Copy(Getattr((yyval.node), "class_rename"));
		       add_symbols((yyval.node));
		       if (!cparse_cplusplusout)
			 Delattr((yyval.node), "nested:outer");
		       Delattr((yyval.node), "class_rename");
		       (yyval.node) = 0;
		     } else {
		       yyrename = Copy(Getattr((yyval.node), "class_rename"));
		       add_symbols((yyval.node));
		       add_symbols((yyvsp[0].node));
		       Delattr((yyval.node), "class_rename");
		     }
		   }
		   Delete(ty);
		   Swig_symbol_setscope(cscope);
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   Classprefix = currentOuterClass ? Getattr(currentOuterClass, "Classprefix") : 0;
		   Delete((yyvsp[-9].str));
	       }
#line 7977 "CParse/parser.c"
    break;

  case 164: /* @6: %empty  */
#line 4215 "./CParse/parser.y"
                                                          {
	       String *unnamed;
	       String *code;
	       unnamed = make_unnamed();
	       (yyval.node) = new_node("class");
	       Setattr((yyval.node),"kind",(yyvsp[-2].type));
	       if ((yyvsp[-1].bases)) {
		 Setattr((yyval.node),"baselist", Getattr((yyvsp[-1].bases),"public"));
		 Setattr((yyval.node),"protectedbaselist", Getattr((yyvsp[-1].bases),"protected"));
		 Setattr((yyval.node),"privatebaselist", Getattr((yyvsp[-1].bases),"private"));
	       }
	       Setattr((yyval.node),"storage",(yyvsp[-3].str));
	       Setattr((yyval.node),"unnamed",unnamed);
	       Setattr((yyval.node),"allows_typedef","1");

	       /* Temporary unofficial symtab for use until add_symbols() adds "sym:symtab" */
	       Setattr((yyval.node), "unofficial:symtab", Swig_symbol_current());

	       if (currentOuterClass) {
		 SetFlag((yyval.node), "nested");
		 Setattr((yyval.node), "nested:outer", currentOuterClass);
		 set_access_mode((yyval.node));
	       }
	       Swig_features_get(Swig_cparse_features(), Namespaceprefix, 0, 0, (yyval.node));
	       /* save yyrename to the class attribute, to be used later in add_symbols()*/
	       Setattr((yyval.node), "class_rename", make_name((yyval.node),0,0));
	       if (Strcmp((yyvsp[-2].type), "class") == 0) {
		 cplus_mode = CPLUS_PRIVATE;
	       } else {
		 cplus_mode = CPLUS_PUBLIC;
	       }
	       Swig_symbol_newscope();
	       cparse_start_line = cparse_line;
	       currentOuterClass = (yyval.node);
	       inclass = 1;
	       Classprefix = 0;
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       /* save the structure declaration to make a typedef for it later*/
	       code = get_raw_text_balanced('{', '}');
	       Setattr((yyval.node), "code", code);
	       Delete(code);
	     }
#line 8025 "CParse/parser.c"
    break;

  case 165: /* cpp_class_decl: storage_class cpptype inherit LBRACE @6 cpp_members RBRACE cpp_opt_declarators  */
#line 4257 "./CParse/parser.y"
                                                            {
	       String *unnamed;
               List *bases = 0;
	       String *name = 0;
	       Node *n;
	       Symtab *cscope;
	       Classprefix = 0;
	       (void)(yyvsp[-3].node);
	       (yyval.node) = currentOuterClass;
	       currentOuterClass = Getattr((yyval.node), "nested:outer");
	       if (!currentOuterClass)
		 inclass = 0;
	       else
		 restore_access_mode((yyval.node));

	       cscope = Getattr((yyval.node), "unofficial:symtab");
	       Delattr((yyval.node), "unofficial:symtab");

	       unnamed = Getattr((yyval.node),"unnamed");
               /* Check for pure-abstract class */
	       Setattr((yyval.node),"abstracts", pure_abstracts((yyvsp[-2].node)));
	       n = (yyvsp[0].node);
	       if (cparse_cplusplus && currentOuterClass && ignore_nested_classes && !GetFlag((yyval.node), "feature:flatnested")) {
		 String *name = n ? Copy(Getattr(n, "name")) : 0;
		 (yyval.node) = nested_forward_declaration((yyvsp[-7].str), (yyvsp[-6].type), 0, name, n);
	       } else if (n) {
	         appendSibling((yyval.node),n);
		 /* If a proper typedef name was given, we'll use it to set the scope name */
		 name = try_to_find_a_name_for_unnamed_structure((yyvsp[-7].str), n);
		 if (name) {
		   String *scpname = 0;
		   SwigType *ty;
		   Setattr((yyval.node),"tdname",name);
		   Setattr((yyval.node),"name",name);
		   Swig_symbol_setscopename(name);
		   if ((yyvsp[-5].bases))
		     bases = Swig_make_inherit_list(name,Getattr((yyvsp[-5].bases),"public"),Namespaceprefix);
		   Swig_inherit_base_symbols(bases);

		     /* If a proper name was given, we use that as the typedef, not unnamed */
		   Clear(unnamed);
		   Append(unnamed, name);
		   if (cparse_cplusplus && !cparse_externc) {
		     ty = NewString(name);
		   } else {
		     ty = NewStringf("%s %s", (yyvsp[-6].type),name);
		   }
		   while (n) {
		     Setattr(n,"storage",(yyvsp[-7].str));
		     Setattr(n, "type", ty);
		     if (!cparse_cplusplus && currentOuterClass && (!Getattr(currentOuterClass, "name"))) {
		       SetFlag(n,"hasconsttype");
		     }
		     n = nextSibling(n);
		   }
		   n = (yyvsp[0].node);

		   /* Check for previous extensions */
		   {
		     String *clsname = Swig_symbol_qualifiedscopename(0);
		     Node *am = Getattr(Swig_extend_hash(),clsname);
		     if (am) {
		       /* Merge the extension into the symbol table */
		       Swig_extend_merge((yyval.node),am);
		       Swig_extend_append_previous((yyval.node),am);
		       Delattr(Swig_extend_hash(),clsname);
		     }
		     Delete(clsname);
		   }
		   if (!classes) classes = NewHash();
		   scpname = Swig_symbol_qualifiedscopename(0);
		   Setattr(classes,scpname,(yyval.node));
		   Delete(scpname);
		 } else { /* no suitable name was found for a struct */
		   Setattr((yyval.node), "nested:unnamed", Getattr(n, "name")); /* save the name of the first declarator for later use in name generation*/
		   while (n) { /* attach unnamed struct to the declarators, so that they would receive proper type later*/
		     Setattr(n, "nested:unnamedtype", (yyval.node));
		     Setattr(n, "storage", (yyvsp[-7].str));
		     n = nextSibling(n);
		   }
		   n = (yyvsp[0].node);
		   Swig_symbol_setscopename("<unnamed>");
		 }
		 appendChild((yyval.node),(yyvsp[-2].node));
		 /* Pop the scope */
		 Setattr((yyval.node),"symtab",Swig_symbol_popscope());
		 if (name) {
		   Delete(yyrename);
		   yyrename = Copy(Getattr((yyval.node), "class_rename"));
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   add_symbols((yyval.node));
		   add_symbols(n);
		   Delattr((yyval.node), "class_rename");
		 } else if (cparse_cplusplus)
		   (yyval.node) = 0; /* ignore unnamed structs for C++ */
		   Delete(unnamed);
	       } else { /* unnamed struct without declarator*/
		 Swig_symbol_popscope();
	         Delete(Namespaceprefix);
		 Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		 add_symbols((yyvsp[-2].node));
		 Delete((yyval.node));
		 (yyval.node) = (yyvsp[-2].node); /* pass member list to outer class/namespace (instead of self)*/
	       }
	       Swig_symbol_setscope(cscope);
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       Classprefix = currentOuterClass ? Getattr(currentOuterClass, "Classprefix") : 0;
	       Delete((yyvsp[-7].str));
              }
#line 8141 "CParse/parser.c"
    break;

  case 166: /* cpp_opt_declarators: SEMI  */
#line 4370 "./CParse/parser.y"
                            { (yyval.node) = 0; }
#line 8147 "CParse/parser.c"
    break;

  case 167: /* cpp_opt_declarators: declarator cpp_const initializer c_decl_tail  */
#line 4371 "./CParse/parser.y"
                                                                    {
                        (yyval.node) = new_node("cdecl");
                        Setattr((yyval.node),"name",(yyvsp[-3].decl).id);
                        Setattr((yyval.node),"decl",(yyvsp[-3].decl).type);
                        Setattr((yyval.node),"parms",(yyvsp[-3].decl).parms);
			set_nextSibling((yyval.node), (yyvsp[0].node));
                    }
#line 8159 "CParse/parser.c"
    break;

  case 168: /* cpp_forward_class_decl: storage_class cpptype idcolon SEMI  */
#line 4383 "./CParse/parser.y"
                                                            {
	      if ((yyvsp[-3].str) && Strstr((yyvsp[-3].str), "friend")) {
		/* Ignore */
                (yyval.node) = 0; 
	      } else {
		(yyval.node) = new_node("classforward");
		Setattr((yyval.node),"kind",(yyvsp[-2].type));
		Setattr((yyval.node),"name",(yyvsp[-1].str));
		Setattr((yyval.node),"sym:weak", "1");
		add_symbols((yyval.node));
	      }
	      Delete((yyvsp[-3].str));
             }
#line 8177 "CParse/parser.c"
    break;

  case 169: /* $@7: %empty  */
#line 4402 "./CParse/parser.y"
                                                                 { 
		    if (currentOuterClass)
		      Setattr(currentOuterClass, "template_parameters", template_parameters);
		    template_parameters = (yyvsp[-1].tparms); 
		    parsing_template_declaration = 1;
		  }
#line 8188 "CParse/parser.c"
    break;

  case 170: /* cpp_template_decl: TEMPLATE LESSTHAN template_parms GREATERTHAN $@7 cpp_template_possible  */
#line 4407 "./CParse/parser.y"
                                          {
			String *tname = 0;
			int     error = 0;

			/* check if we get a namespace node with a class declaration, and retrieve the class */
			Symtab *cscope = Swig_symbol_current();
			Symtab *sti = 0;
			Node *ntop = (yyvsp[0].node);
			Node *ni = ntop;
			SwigType *ntype = ni ? nodeType(ni) : 0;
			while (ni && Strcmp(ntype,"namespace") == 0) {
			  sti = Getattr(ni,"symtab");
			  ni = firstChild(ni);
			  ntype = nodeType(ni);
			}
			if (sti) {
			  Swig_symbol_setscope(sti);
			  Delete(Namespaceprefix);
			  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
			  (yyval.node) = ni;
			} else {
			  (yyval.node) = (yyvsp[0].node);
			}

			if ((yyval.node)) tname = Getattr((yyval.node),"name");
			
			/* Check if the class is a template specialization */
			if (((yyval.node)) && (Strchr(tname,'<')) && (!is_operator(tname))) {
			  /* If a specialization.  Check if defined. */
			  Node *tempn = 0;
			  {
			    String *tbase = SwigType_templateprefix(tname);
			    tempn = Swig_symbol_clookup_local(tbase,0);
			    if (!tempn || (Strcmp(nodeType(tempn),"template") != 0)) {
			      SWIG_WARN_NODE_BEGIN(tempn);
			      Swig_warning(WARN_PARSE_TEMPLATE_SP_UNDEF, Getfile((yyval.node)),Getline((yyval.node)),"Specialization of non-template '%s'.\n", tbase);
			      SWIG_WARN_NODE_END(tempn);
			      tempn = 0;
			      error = 1;
			    }
			    Delete(tbase);
			  }
			  Setattr((yyval.node),"specialization","1");
			  Setattr((yyval.node),"templatetype",nodeType((yyval.node)));
			  set_nodeType((yyval.node),"template");
			  /* Template partial specialization */
			  if (tempn && ((yyvsp[-3].tparms)) && ((yyval.node))) {
			    ParmList *primary_templateparms = Getattr(tempn, "templateparms");
			    String *targs = SwigType_templateargs(tname); /* tname contains name and specialized template parameters, for example: X<(p.T,TT)> */
			    List *tlist = SwigType_parmlist(targs);
			    int specialization_parms_len = Len(tlist);
			    if (!Getattr((yyval.node),"sym:weak")) {
			      Setattr((yyval.node),"sym:typename","1");
			    }
			    Setattr((yyval.node), "primarytemplate", tempn);
			    Setattr((yyval.node), "templateparms", (yyvsp[-3].tparms));
			    Delattr((yyval.node), "specialization");
			    Setattr((yyval.node), "partialspecialization", "1");
			    
			    if (specialization_parms_len > ParmList_len(primary_templateparms)) {
			      Swig_error(Getfile((yyval.node)), Getline((yyval.node)), "Template partial specialization has more arguments than primary template %d %d.\n", specialization_parms_len, ParmList_len(primary_templateparms));
			      
			    } else if (specialization_parms_len < ParmList_numrequired(primary_templateparms)) {
			      Swig_error(Getfile((yyval.node)), Getline((yyval.node)), "Template partial specialization has fewer arguments than primary template %d %d.\n", specialization_parms_len, ParmList_len(primary_templateparms));
			    } else {
			      /* Create a specialized name with template parameters replaced with $ variables, such as, X<(T1,p.T2) => X<($1,p.$2)> */
			      Parm *p = (yyvsp[-3].tparms);
			      String *fname = NewString(tname);
			      String *ffname = 0;
			      ParmList *partialparms = 0;

			      char   tmp[32];
			      int i = 0;
			      while (p) {
				String *name = Getattr(p,"name");
				++i;
				if (!name) {
				  p = nextSibling(p);
				  continue;
				}
				sprintf(tmp, "$%d", i);
				Replaceid(fname, name, tmp);
				p = nextSibling(p);
			      }
			      /* Patch argument names with typedef */
			      {
				Iterator tt;
				Parm *parm_current = 0;
				List *tparms = SwigType_parmlist(fname);
				ffname = SwigType_templateprefix(fname);
				Append(ffname,"<(");
				for (tt = First(tparms); tt.item; ) {
				  SwigType *rtt = Swig_symbol_typedef_reduce(tt.item,0);
				  SwigType *ttr = Swig_symbol_type_qualify(rtt,0);

				  Parm *newp = NewParmWithoutFileLineInfo(ttr, 0);
				  if (partialparms)
				    set_nextSibling(parm_current, newp);
				  else
				    partialparms = newp;
				  parm_current = newp;

				  Append(ffname,ttr);
				  tt = Next(tt);
				  if (tt.item) Putc(',',ffname);
				  Delete(rtt);
				  Delete(ttr);
				}
				Delete(tparms);
				Append(ffname,")>");
			      }
			      {
				/* Replace each primary template parameter's name and value with $ variables, such as, class Y,class T=Y => class $1,class $2=$1 */
				ParmList *primary_templateparms_copy = CopyParmList(primary_templateparms);
				p = primary_templateparms_copy;
				i = 0;
				while (p) {
				  String *name = Getattr(p, "name");
				  Parm *pp = nextSibling(p);
				  ++i;
				  sprintf(tmp, "$%d", i);
				  while (pp) {
				    Replaceid(Getattr(pp, "value"), name, tmp);
				    pp = nextSibling(pp);
				  }
				  Setattr(p, "name", NewString(tmp));
				  p = nextSibling(p);
				}
				/* Modify partialparms by adding in missing default values ($ variables) from primary template parameters */
				partialparms = Swig_cparse_template_partialargs_expand(partialparms, tempn, primary_templateparms_copy);
				Delete(primary_templateparms_copy);
			      }
			      {
				Node *new_partial = NewHash();
				String *partials = Getattr(tempn,"partials");
				if (!partials) {
				  partials = NewList();
				  Setattr(tempn,"partials",partials);
				  Delete(partials);
				}
				/*			      Printf(stdout,"partial: fname = '%s', '%s'\n", fname, Swig_symbol_typedef_reduce(fname,0)); */
				Setattr(new_partial, "partialparms", partialparms);
				Setattr(new_partial, "templcsymname", ffname);
				Append(partials, new_partial);
			      }
			      Setattr((yyval.node),"partialargs",ffname);
			      Swig_symbol_cadd(ffname,(yyval.node));
			    }
			    Delete(tlist);
			    Delete(targs);
			  } else {
			    /* An explicit template specialization */
			    /* add default args from primary (unspecialized) template */
			    String *ty = Swig_symbol_template_deftype(tname,0);
			    String *fname = Swig_symbol_type_qualify(ty,0);
			    Swig_symbol_cadd(fname,(yyval.node));
			    Delete(ty);
			    Delete(fname);
			  }
			} else if ((yyval.node)) {
			  Setattr((yyval.node), "templatetype", nodeType((yyval.node)));
			  set_nodeType((yyval.node),"template");
			  Setattr((yyval.node),"templateparms", (yyvsp[-3].tparms));
			  if (!Getattr((yyval.node),"sym:weak")) {
			    Setattr((yyval.node),"sym:typename","1");
			  }
			  add_symbols((yyval.node));
			  default_arguments((yyval.node));
			  /* We also place a fully parameterized version in the symbol table */
			  {
			    Parm *p;
			    String *fname = NewStringf("%s<(", Getattr((yyval.node),"name"));
			    p = (yyvsp[-3].tparms);
			    while (p) {
			      String *n = Getattr(p,"name");
			      if (!n) n = Getattr(p,"type");
			      Append(fname,n);
			      p = nextSibling(p);
			      if (p) Putc(',',fname);
			    }
			    Append(fname,")>");
			    Swig_symbol_cadd(fname,(yyval.node));
			  }
			}
			(yyval.node) = ntop;
			Swig_symbol_setscope(cscope);
			Delete(Namespaceprefix);
			Namespaceprefix = Swig_symbol_qualifiedscopename(0);
			if (error || (nscope_inner && Strcmp(nodeType(nscope_inner), "class") == 0)) {
			  (yyval.node) = 0;
			}
			if (currentOuterClass)
			  template_parameters = Getattr(currentOuterClass, "template_parameters");
			else
			  template_parameters = 0;
			parsing_template_declaration = 0;
                }
#line 8390 "CParse/parser.c"
    break;

  case 171: /* cpp_template_decl: TEMPLATE cpptype idcolon  */
#line 4606 "./CParse/parser.y"
                                           {
		  Swig_warning(WARN_PARSE_EXPLICIT_TEMPLATE, cparse_file, cparse_line, "Explicit template instantiation ignored.\n");
                  (yyval.node) = 0; 
		}
#line 8399 "CParse/parser.c"
    break;

  case 172: /* cpp_template_decl: TEMPLATE cpp_alternate_rettype idcolon LPAREN parms RPAREN  */
#line 4612 "./CParse/parser.y"
                                                                             {
			Swig_warning(WARN_PARSE_EXPLICIT_TEMPLATE, cparse_file, cparse_line, "Explicit template instantiation ignored.\n");
                  (yyval.node) = 0; 
		}
#line 8408 "CParse/parser.c"
    break;

  case 173: /* cpp_template_decl: EXTERN TEMPLATE cpptype idcolon  */
#line 4618 "./CParse/parser.y"
                                                  {
		  Swig_warning(WARN_PARSE_EXTERN_TEMPLATE, cparse_file, cparse_line, "Extern template ignored.\n");
                  (yyval.node) = 0; 
                }
#line 8417 "CParse/parser.c"
    break;

  case 174: /* cpp_template_decl: EXTERN TEMPLATE cpp_alternate_rettype idcolon LPAREN parms RPAREN  */
#line 4624 "./CParse/parser.y"
                                                                                    {
			Swig_warning(WARN_PARSE_EXTERN_TEMPLATE, cparse_file, cparse_line, "Extern template ignored.\n");
                  (yyval.node) = 0; 
		}
#line 8426 "CParse/parser.c"
    break;

  case 178: /* cpp_template_possible: cpp_template_decl  */
#line 4633 "./CParse/parser.y"
                                    {
		  (yyval.node) = 0;
                }
#line 8434 "CParse/parser.c"
    break;

  case 181: /* template_parms: template_parms_builder  */
#line 4640 "./CParse/parser.y"
                                        {
		 (yyval.tparms) = (yyvsp[0].pbuilder).parms;
	       }
#line 8442 "CParse/parser.c"
    break;

  case 182: /* template_parms: %empty  */
#line 4643 "./CParse/parser.y"
                        {
		 (yyval.tparms) = 0;
	       }
#line 8450 "CParse/parser.c"
    break;

  case 183: /* template_parms_builder: templateparameter  */
#line 4648 "./CParse/parser.y"
                                           {
		    (yyval.pbuilder).parms = (yyval.pbuilder).last = (yyvsp[0].p);
		  }
#line 8458 "CParse/parser.c"
    break;

  case 184: /* template_parms_builder: template_parms_builder COMMA templateparameter  */
#line 4651 "./CParse/parser.y"
                                                                       {
		    // Build a linked list in the order specified, but avoiding
		    // a right recursion rule because "Right recursion uses up
		    // space on the Bison stack in proportion to the number of
		    // elements in the sequence".
		    set_nextSibling((yyvsp[-2].pbuilder).last, (yyvsp[0].p));
		    (yyval.pbuilder).parms = (yyvsp[-2].pbuilder).parms;
		    (yyval.pbuilder).last = (yyvsp[0].p);
		  }
#line 8472 "CParse/parser.c"
    break;

  case 185: /* templateparameter: templcpptype def_args  */
#line 4662 "./CParse/parser.y"
                                          {
		    (yyval.p) = NewParmWithoutFileLineInfo((yyvsp[-1].type), 0);
		    Setfile((yyval.p), cparse_file);
		    Setline((yyval.p), cparse_line);
		    Setattr((yyval.p), "value", (yyvsp[0].dtype).val);
		    if ((yyvsp[0].dtype).stringval) Setattr((yyval.p), "stringval", (yyvsp[0].dtype).stringval);
		    if ((yyvsp[0].dtype).numval) Setattr((yyval.p), "numval", (yyvsp[0].dtype).numval);
		  }
#line 8485 "CParse/parser.c"
    break;

  case 186: /* templateparameter: TEMPLATE LESSTHAN template_parms GREATERTHAN cpptype idcolon def_args  */
#line 4670 "./CParse/parser.y"
                                                                                          {
		    (yyval.p) = NewParmWithoutFileLineInfo(NewStringf("template< %s > %s %s", ParmList_str_defaultargs((yyvsp[-4].tparms)), (yyvsp[-2].type), (yyvsp[-1].str)), (yyvsp[-1].str));
		    Setfile((yyval.p), cparse_file);
		    Setline((yyval.p), cparse_line);
		    if ((yyvsp[0].dtype).val) {
		      Setattr((yyval.p), "value", (yyvsp[0].dtype).val);
		    }
		  }
#line 8498 "CParse/parser.c"
    break;

  case 187: /* templateparameter: TEMPLATE LESSTHAN template_parms GREATERTHAN cpptype def_args  */
#line 4678 "./CParse/parser.y"
                                                                                  {
		    (yyval.p) = NewParmWithoutFileLineInfo(NewStringf("template< %s > %s", ParmList_str_defaultargs((yyvsp[-3].tparms)), (yyvsp[-1].type)), 0);
		    Setfile((yyval.p), cparse_file);
		    Setline((yyval.p), cparse_line);
		    if ((yyvsp[0].dtype).val) {
		      Setattr((yyval.p), "value", (yyvsp[0].dtype).val);
		    }
		  }
#line 8511 "CParse/parser.c"
    break;

  case 188: /* templateparameter: parm  */
#line 4686 "./CParse/parser.y"
                         {
		    Parm *p = (yyvsp[0].p);
		    String *name = Getattr(p, "name");
		    (yyval.p) = (yyvsp[0].p);

		    /* Correct the 'type name' parameter string, split into the appropriate "name" and "type" attributes */
		    if (!name) {
		      String *type = Getattr(p, "type");
		      if ((Strncmp(type, "class ", 6) == 0) || (Strncmp(type, "typename ", 9) == 0)) {
			/* A 'class T' parameter */
			const char *t = Strchr(type, ' ');
			Setattr(p, "name", t + 1);
			Setattr(p, "type", NewStringWithSize(type, (int)(t - Char(type))));
		      } else if ((Strncmp(type, "v.class ", 8) == 0) || (Strncmp(type, "v.typename ", 11) == 0)) {
			/* Variadic template args */
			const char *t = Strchr(type, ' ');
			Setattr(p, "name", t + 1);
			Setattr(p, "type", NewStringWithSize(type, (int)(t - Char(type))));
		      }
		    }
                  }
#line 8537 "CParse/parser.c"
    break;

  case 189: /* cpp_using_decl: USING idcolon SEMI  */
#line 4711 "./CParse/parser.y"
                                    {
                  String *uname = Swig_symbol_type_qualify((yyvsp[-1].str),0);
                  /* Possible TODO: In testcase using_member_multiple_inherit class Susing3, uname is "Susing1::usingmethod" instead of "Susing2::usingmethod" */
		  String *name = Swig_scopename_last((yyvsp[-1].str));
                  (yyval.node) = new_node("using");
		  Setattr((yyval.node),"uname",uname);
		  Setattr((yyval.node),"name", name);
		  Delete(uname);
		  Delete(name);
		  add_symbols((yyval.node));
             }
#line 8553 "CParse/parser.c"
    break;

  case 190: /* cpp_using_decl: USING TYPENAME idcolon SEMI  */
#line 4722 "./CParse/parser.y"
                                           {
		  String *uname = Swig_symbol_type_qualify((yyvsp[-1].str),0);
		  String *name = Swig_scopename_last((yyvsp[-1].str));
		  (yyval.node) = new_node("using");
		  Setattr((yyval.node),"uname",uname);
		  Setattr((yyval.node),"name", name);
		  Delete(uname);
		  Delete(name);
		  add_symbols((yyval.node));
	     }
#line 8568 "CParse/parser.c"
    break;

  case 191: /* cpp_using_decl: USING NAMESPACE idcolon SEMI  */
#line 4732 "./CParse/parser.y"
                                            {
	       Node *n = Swig_symbol_clookup((yyvsp[-1].str),0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Nothing known about namespace '%s'\n", SwigType_namestr((yyvsp[-1].str)));
		 (yyval.node) = 0;
	       } else {

		 while (Strcmp(nodeType(n),"using") == 0) {
		   n = Getattr(n,"node");
		 }
		 if (n) {
		   if (Strcmp(nodeType(n),"namespace") == 0) {
		     Symtab *current = Swig_symbol_current();
		     Symtab *symtab = Getattr(n,"symtab");
		     (yyval.node) = new_node("using");
		     Setattr((yyval.node),"node",n);
		     Setattr((yyval.node),"namespace", (yyvsp[-1].str));
		     if (current != symtab) {
		       Swig_symbol_inherit(symtab);
		     }
		   } else {
		     Swig_error(cparse_file, cparse_line, "'%s' is not a namespace.\n", SwigType_namestr((yyvsp[-1].str)));
		     (yyval.node) = 0;
		   }
		 } else {
		   (yyval.node) = 0;
		 }
	       }
             }
#line 8602 "CParse/parser.c"
    break;

  case 192: /* @8: %empty  */
#line 4763 "./CParse/parser.y"
                                                    {
                Hash *h;
		Node *parent_ns = 0;
		List *scopes = Swig_scopename_tolist((yyvsp[-1].str));
		int ilen = Len(scopes);
		int i;

/*
Printf(stdout, "==== Namespace %s creation...\n", $idcolon);
*/
		(yyval.node) = 0;
		for (i = 0; i < ilen; i++) {
		  Node *ns = new_node("namespace");
		  Symtab *current_symtab = Swig_symbol_current();
		  String *scopename = Getitem(scopes, i);
		  Setattr(ns, "name", scopename);
		  (yyval.node) = ns;
		  if (parent_ns)
		    appendChild(parent_ns, ns);
		  parent_ns = ns;
		  h = Swig_symbol_clookup(scopename, 0);
		  if (h && (current_symtab == Getattr(h, "sym:symtab")) && (Strcmp(nodeType(h), "namespace") == 0)) {
/*
Printf(stdout, "  Scope %s [found C++17 style]\n", scopename);
*/
		    if (Getattr(h, "alias")) {
		      h = Getattr(h, "namespace");
		      Swig_warning(WARN_PARSE_NAMESPACE_ALIAS, cparse_file, cparse_line, "Namespace alias '%s' not allowed here. Assuming '%s'\n",
				   scopename, Getattr(h, "name"));
		      scopename = Getattr(h, "name");
		    }
		    Swig_symbol_setscope(Getattr(h, "symtab"));
		  } else {
/*
Printf(stdout, "  Scope %s [creating single scope C++17 style]\n", scopename);
*/
		    h = Swig_symbol_newscope();
		    Swig_symbol_setscopename(scopename);
		  }
		  Delete(Namespaceprefix);
		  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		}
		Delete(scopes);
             }
#line 8651 "CParse/parser.c"
    break;

  case 193: /* cpp_namespace_decl: NAMESPACE idcolon LBRACE @8 interface RBRACE  */
#line 4806 "./CParse/parser.y"
                                      {
		Node *n = (yyvsp[-2].node);
		Node *top_ns = 0;
		do {
		  Setattr(n, "symtab", Swig_symbol_popscope());
		  Delete(Namespaceprefix);
		  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		  add_symbols(n);
		  top_ns = n;
		  n = parentNode(n);
		} while(n);
		appendChild((yyvsp[-2].node), firstChild((yyvsp[-1].node)));
		Delete((yyvsp[-1].node));
		(yyval.node) = top_ns;
             }
#line 8671 "CParse/parser.c"
    break;

  case 194: /* @9: %empty  */
#line 4821 "./CParse/parser.y"
                                      {
	       Hash *h;
	       (yyval.node) = Swig_symbol_current();
	       h = Swig_symbol_clookup("    ",0);
	       if (h && (Strcmp(nodeType(h),"namespace") == 0)) {
		 Swig_symbol_setscope(Getattr(h,"symtab"));
	       } else {
		 Swig_symbol_newscope();
		 /* we don't use "__unnamed__", but a long 'empty' name */
		 Swig_symbol_setscopename("    ");
	       }
	       Namespaceprefix = 0;
             }
#line 8689 "CParse/parser.c"
    break;

  case 195: /* cpp_namespace_decl: NAMESPACE LBRACE @9 interface RBRACE  */
#line 4833 "./CParse/parser.y"
                                      {
	       (yyval.node) = (yyvsp[-1].node);
	       set_nodeType((yyval.node),"namespace");
	       Setattr((yyval.node),"unnamed","1");
	       Setattr((yyval.node),"symtab", Swig_symbol_popscope());
	       Swig_symbol_setscope((yyvsp[-2].node));
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols((yyval.node));
             }
#line 8704 "CParse/parser.c"
    break;

  case 196: /* cpp_namespace_decl: NAMESPACE identifier EQUAL idcolon SEMI  */
#line 4843 "./CParse/parser.y"
                                                       {
	       /* Namespace alias */
	       Node *n;
	       (yyval.node) = new_node("namespace");
	       Setattr((yyval.node),"name",(yyvsp[-3].id));
	       Setattr((yyval.node),"alias",(yyvsp[-1].str));
	       n = Swig_symbol_clookup((yyvsp[-1].str),0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Unknown namespace '%s'\n", SwigType_namestr((yyvsp[-1].str)));
		 (yyval.node) = 0;
	       } else {
		 if (Strcmp(nodeType(n),"namespace") != 0) {
		   Swig_error(cparse_file, cparse_line, "'%s' is not a namespace\n", SwigType_namestr((yyvsp[-1].str)));
		   (yyval.node) = 0;
		 } else {
		   while (Getattr(n,"alias")) {
		     n = Getattr(n,"namespace");
		   }
		   Setattr((yyval.node),"namespace",n);
		   add_symbols((yyval.node));
		   /* Set up a scope alias */
		   Swig_symbol_alias((yyvsp[-3].id),Getattr(n,"symtab"));
		 }
	       }
             }
#line 8734 "CParse/parser.c"
    break;

  case 197: /* cpp_members: cpp_members_builder  */
#line 4870 "./CParse/parser.y"
                                  {
		 (yyval.node) = (yyvsp[0].nodebuilder).node;
	       }
#line 8742 "CParse/parser.c"
    break;

  case 198: /* cpp_members: cpp_members_builder DOXYGENSTRING  */
#line 4873 "./CParse/parser.y"
                                                   {
		 /* Quietly ignore misplaced doxygen string after a member, like Doxygen does */
		 (yyval.node) = (yyvsp[-1].nodebuilder).node;
		 Delete((yyvsp[0].str));
	       }
#line 8752 "CParse/parser.c"
    break;

  case 199: /* cpp_members: %empty  */
#line 4878 "./CParse/parser.y"
                        {
		 (yyval.node) = 0;
	       }
#line 8760 "CParse/parser.c"
    break;

  case 200: /* cpp_members: DOXYGENSTRING  */
#line 4881 "./CParse/parser.y"
                               {
		 /* Quietly ignore misplaced doxygen string in empty class, like Doxygen does */
		 (yyval.node) = 0;
		 Delete((yyvsp[0].str));
	       }
#line 8770 "CParse/parser.c"
    break;

  case 201: /* cpp_members: error  */
#line 4886 "./CParse/parser.y"
                       {
		 Swig_error(cparse_file, cparse_line, "Syntax error in input(3).\n");
		 Exit(EXIT_FAILURE);
	       }
#line 8779 "CParse/parser.c"
    break;

  case 202: /* cpp_members_builder: cpp_member  */
#line 4892 "./CParse/parser.y"
                                 {
	     (yyval.nodebuilder).node = (yyval.nodebuilder).last = (yyvsp[0].node);
	   }
#line 8787 "CParse/parser.c"
    break;

  case 203: /* cpp_members_builder: cpp_members_builder cpp_member  */
#line 4895 "./CParse/parser.y"
                                                {
	     // Build a linked list in the order specified, but avoiding
	     // a right recursion rule because "Right recursion uses up
	     // space on the Bison stack in proportion to the number of
	     // elements in the sequence".
	     if ((yyvsp[0].node)) {
	       if ((yyvsp[-1].nodebuilder).node) {
		 Node *last = (yyvsp[-1].nodebuilder).last;
		 /* Advance to the last sibling. */
		 for (Node *p = last; p; p = nextSibling(p)) {
		   last = p;
		 }
		 set_nextSibling(last, (yyvsp[0].node));
		 set_previousSibling((yyvsp[0].node), last);
		 (yyval.nodebuilder).node = (yyvsp[-1].nodebuilder).node;
	       } else {
		 (yyval.nodebuilder).node = (yyval.nodebuilder).last = (yyvsp[0].node);
	       }
	     } else {
	       (yyval.nodebuilder) = (yyvsp[-1].nodebuilder);
	     }
	   }
#line 8814 "CParse/parser.c"
    break;

  case 205: /* cpp_member_no_dox: cpp_constructor_decl  */
#line 4926 "./CParse/parser.y"
                                    { 
                 (yyval.node) = (yyvsp[0].node); 
		 if (extendmode && current_class) {
		   String *symname;
		   symname= make_name((yyval.node),Getattr((yyval.node),"name"), Getattr((yyval.node),"decl"));
		   if (Strcmp(symname,Getattr((yyval.node),"name")) == 0) {
		     /* No renaming operation.  Set name to class name */
		     Delete(yyrename);
		     yyrename = NewString(Getattr(current_class,"sym:name"));
		   } else {
		     Delete(yyrename);
		     yyrename = symname;
		   }
		 }
		 add_symbols((yyval.node));
                 default_arguments((yyval.node));
             }
#line 8836 "CParse/parser.c"
    break;

  case 212: /* cpp_member_no_dox: storage_class idcolon SEMI  */
#line 4949 "./CParse/parser.y"
                                          { (yyval.node) = 0; Delete((yyvsp[-2].str)); }
#line 8842 "CParse/parser.c"
    break;

  case 219: /* cpp_member_no_dox: anonymous_bitfield  */
#line 4956 "./CParse/parser.y"
                                  { (yyval.node) = 0; }
#line 8848 "CParse/parser.c"
    break;

  case 222: /* cpp_member_no_dox: SEMI  */
#line 4959 "./CParse/parser.y"
                    { (yyval.node) = 0; }
#line 8854 "CParse/parser.c"
    break;

  case 224: /* cpp_member: DOXYGENSTRING cpp_member_no_dox  */
#line 4962 "./CParse/parser.y"
                                               {
	         (yyval.node) = (yyvsp[0].node);
		 set_comment((yyvsp[0].node), (yyvsp[-1].str));
	     }
#line 8863 "CParse/parser.c"
    break;

  case 225: /* cpp_member: cpp_member_no_dox DOXYGENPOSTSTRING  */
#line 4966 "./CParse/parser.y"
                                                   {
	         (yyval.node) = (yyvsp[-1].node);
		 set_comment((yyvsp[-1].node), (yyvsp[0].str));
	     }
#line 8872 "CParse/parser.c"
    break;

  case 226: /* $@10: %empty  */
#line 4970 "./CParse/parser.y"
                             {
	       extendmode = 1;
	       if (cplus_mode != CPLUS_PUBLIC) {
		 Swig_error(cparse_file,cparse_line,"%%extend can only be used in a public section\n");
	       }
	     }
#line 8883 "CParse/parser.c"
    break;

  case 227: /* cpp_member: EXTEND LBRACE $@10 cpp_members RBRACE  */
#line 4975 "./CParse/parser.y"
                                  {
	       extendmode = 0;
	       (yyval.node) = new_node("extend");
	       mark_nodes_as_extend((yyvsp[-1].node));
	       appendChild((yyval.node), (yyvsp[-1].node));
	     }
#line 8894 "CParse/parser.c"
    break;

  case 228: /* cpp_constructor_decl: storage_class type LPAREN parms RPAREN ctor_end  */
#line 4989 "./CParse/parser.y"
                                                                       {
	      /* Cannot be a constructor declaration/definition if parsed as a friend destructor/constructor
	         or a badly declared friend function without return type */
	      int isfriend = Strstr((yyvsp[-5].str), "friend") != NULL;
	      if (!isfriend && (inclass || extendmode)) {
	        String *name = SwigType_templateprefix((yyvsp[-4].type)); /* A constructor can optionally be declared with template parameters before C++20, strip these off */
		SwigType *decl = NewStringEmpty();
		(yyval.node) = new_node("constructor");
		Setattr((yyval.node),"storage",(yyvsp[-5].str));
		Setattr((yyval.node), "name", name);
		Setattr((yyval.node),"parms",(yyvsp[-2].pl));
		SwigType_add_function(decl,(yyvsp[-2].pl));
		Setattr((yyval.node),"decl",decl);
		Setattr((yyval.node),"throws",(yyvsp[0].decl).throws);
		Setattr((yyval.node),"throw",(yyvsp[0].decl).throwf);
		Setattr((yyval.node),"noexcept",(yyvsp[0].decl).nexcept);
		Setattr((yyval.node),"final",(yyvsp[0].decl).final);
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr((yyval.node),"code",code);
		  Delete(code);
		}
		SetFlag((yyval.node),"feature:new");
		if ((yyvsp[0].decl).defarg)
		  Setattr((yyval.node), "value", (yyvsp[0].decl).defarg);
		if ((yyvsp[0].decl).stringdefarg)
		  Setattr((yyval.node), "stringval", (yyvsp[0].decl).stringdefarg);
		if ((yyvsp[0].decl).numdefarg)
		  Setattr((yyval.node), "numval", (yyvsp[0].decl).numdefarg);
	      } else {
		(yyval.node) = 0;
              }
	      Delete((yyvsp[-5].str));
              }
#line 8933 "CParse/parser.c"
    break;

  case 229: /* cpp_destructor_decl: storage_class NOT idtemplate LPAREN parms RPAREN cpp_vend  */
#line 5027 "./CParse/parser.y"
                                                                                {
	       String *name = SwigType_templateprefix((yyvsp[-4].str)); /* A destructor can optionally be declared with template parameters before C++20, strip these off */
	       Insert(name, 0, "~");
	       (yyval.node) = new_node("destructor");
	       Setattr((yyval.node), "storage", (yyvsp[-6].str));
	       Setattr((yyval.node), "name", name);
	       Delete(name);
	       if (Len(scanner_ccode)) {
		 String *code = Copy(scanner_ccode);
		 Setattr((yyval.node), "code", code);
		 Delete(code);
	       }
	       {
		 String *decl = NewStringEmpty();
		 SwigType_add_function(decl, (yyvsp[-2].pl));
		 Setattr((yyval.node), "decl", decl);
		 Delete(decl);
	       }
	       Setattr((yyval.node), "throws", (yyvsp[0].dtype).throws);
	       Setattr((yyval.node), "throw", (yyvsp[0].dtype).throwf);
	       Setattr((yyval.node), "noexcept", (yyvsp[0].dtype).nexcept);
	       Setattr((yyval.node), "final", (yyvsp[0].dtype).final);
	       if ((yyvsp[0].dtype).val) {
		 if (Equal((yyvsp[0].dtype).val, "0")) {
		   if (!Strstr((yyvsp[-6].str), "virtual"))
		     Swig_error(cparse_file, cparse_line, "Destructor %s uses a pure specifier but is not virtual.\n", Swig_name_decl((yyval.node)));
		 } else if (!(Equal((yyvsp[0].dtype).val, "delete") || Equal((yyvsp[0].dtype).val, "default"))) {
		   Swig_error(cparse_file, cparse_line, "Destructor %s has an invalid pure specifier, only = 0 is allowed.\n", Swig_name_decl((yyval.node)));
		 }
		 Setattr((yyval.node), "value", (yyvsp[0].dtype).val);
	       }
	       /* TODO: check all storage decl-specifiers are valid */
	       if ((yyvsp[0].dtype).qualifier)
		 Swig_error(cparse_file, cparse_line, "Destructor %s %s cannot have a qualifier.\n", Swig_name_decl((yyval.node)), SwigType_str((yyvsp[0].dtype).qualifier, 0));
	       add_symbols((yyval.node));
	       Delete((yyvsp[-6].str));
	      }
#line 8975 "CParse/parser.c"
    break;

  case 230: /* cpp_conversion_operator: storage_class CONVERSIONOPERATOR type pointer LPAREN parms RPAREN cpp_vend  */
#line 5068 "./CParse/parser.y"
                                                                                                     {
                 (yyval.node) = new_node("cdecl");
                 Setattr((yyval.node),"type",(yyvsp[-5].type));
		 Setattr((yyval.node),"name",(yyvsp[-6].str));
		 Setattr((yyval.node),"storage",(yyvsp[-7].str));

		 SwigType_add_function((yyvsp[-4].type),(yyvsp[-2].pl));
		 if ((yyvsp[0].dtype).qualifier) {
		   SwigType_push((yyvsp[-4].type),(yyvsp[0].dtype).qualifier);
		 }
		 if ((yyvsp[0].dtype).val) {
		   Setattr((yyval.node),"value",(yyvsp[0].dtype).val);
		 }
		 Setattr((yyval.node),"refqualifier",(yyvsp[0].dtype).refqualifier);
		 Setattr((yyval.node),"decl",(yyvsp[-4].type));
		 Setattr((yyval.node),"parms",(yyvsp[-2].pl));
		 Setattr((yyval.node),"conversion_operator","1");
		 add_symbols((yyval.node));
		 Delete((yyvsp[-6].str));
		 Delete((yyvsp[-7].str));
              }
#line 9001 "CParse/parser.c"
    break;

  case 231: /* cpp_conversion_operator: storage_class CONVERSIONOPERATOR type AND LPAREN parms RPAREN cpp_vend  */
#line 5089 "./CParse/parser.y"
                                                                                        {
		 SwigType *decl;
                 (yyval.node) = new_node("cdecl");
                 Setattr((yyval.node),"type",(yyvsp[-5].type));
		 Setattr((yyval.node),"name",(yyvsp[-6].str));
		 Setattr((yyval.node),"storage",(yyvsp[-7].str));
		 decl = NewStringEmpty();
		 SwigType_add_reference(decl);
		 SwigType_add_function(decl,(yyvsp[-2].pl));
		 if ((yyvsp[0].dtype).qualifier) {
		   SwigType_push(decl,(yyvsp[0].dtype).qualifier);
		 }
		 if ((yyvsp[0].dtype).val) {
		   Setattr((yyval.node),"value",(yyvsp[0].dtype).val);
		 }
		 Setattr((yyval.node),"refqualifier",(yyvsp[0].dtype).refqualifier);
		 Setattr((yyval.node),"decl",decl);
		 Setattr((yyval.node),"parms",(yyvsp[-2].pl));
		 Setattr((yyval.node),"conversion_operator","1");
		 add_symbols((yyval.node));
		 Delete((yyvsp[-6].str));
		 Delete((yyvsp[-7].str));
	       }
#line 9029 "CParse/parser.c"
    break;

  case 232: /* cpp_conversion_operator: storage_class CONVERSIONOPERATOR type LAND LPAREN parms RPAREN cpp_vend  */
#line 5112 "./CParse/parser.y"
                                                                                         {
		 SwigType *decl;
                 (yyval.node) = new_node("cdecl");
                 Setattr((yyval.node),"type",(yyvsp[-5].type));
		 Setattr((yyval.node),"name",(yyvsp[-6].str));
		 Setattr((yyval.node),"storage",(yyvsp[-7].str));
		 decl = NewStringEmpty();
		 SwigType_add_rvalue_reference(decl);
		 SwigType_add_function(decl,(yyvsp[-2].pl));
		 if ((yyvsp[0].dtype).qualifier) {
		   SwigType_push(decl,(yyvsp[0].dtype).qualifier);
		 }
		 if ((yyvsp[0].dtype).val) {
		   Setattr((yyval.node),"value",(yyvsp[0].dtype).val);
		 }
		 Setattr((yyval.node),"refqualifier",(yyvsp[0].dtype).refqualifier);
		 Setattr((yyval.node),"decl",decl);
		 Setattr((yyval.node),"parms",(yyvsp[-2].pl));
		 Setattr((yyval.node),"conversion_operator","1");
		 add_symbols((yyval.node));
		 Delete((yyvsp[-6].str));
		 Delete((yyvsp[-7].str));
	       }
#line 9057 "CParse/parser.c"
    break;

  case 233: /* cpp_conversion_operator: storage_class CONVERSIONOPERATOR type pointer AND LPAREN parms RPAREN cpp_vend  */
#line 5136 "./CParse/parser.y"
                                                                                                {
		 SwigType *decl;
                 (yyval.node) = new_node("cdecl");
                 Setattr((yyval.node),"type",(yyvsp[-6].type));
		 Setattr((yyval.node),"name",(yyvsp[-7].str));
		 Setattr((yyval.node),"storage",(yyvsp[-8].str));
		 decl = NewStringEmpty();
		 SwigType_add_pointer(decl);
		 SwigType_add_reference(decl);
		 SwigType_add_function(decl,(yyvsp[-2].pl));
		 if ((yyvsp[0].dtype).qualifier) {
		   SwigType_push(decl,(yyvsp[0].dtype).qualifier);
		 }
		 if ((yyvsp[0].dtype).val) {
		   Setattr((yyval.node),"value",(yyvsp[0].dtype).val);
		 }
		 Setattr((yyval.node),"refqualifier",(yyvsp[0].dtype).refqualifier);
		 Setattr((yyval.node),"decl",decl);
		 Setattr((yyval.node),"parms",(yyvsp[-2].pl));
		 Setattr((yyval.node),"conversion_operator","1");
		 add_symbols((yyval.node));
		 Delete((yyvsp[-7].str));
		 Delete((yyvsp[-8].str));
	       }
#line 9086 "CParse/parser.c"
    break;

  case 234: /* cpp_conversion_operator: storage_class CONVERSIONOPERATOR type LPAREN parms RPAREN cpp_vend  */
#line 5161 "./CParse/parser.y"
                                                                                   {
		String *t = NewStringEmpty();
		(yyval.node) = new_node("cdecl");
		Setattr((yyval.node),"type",(yyvsp[-4].type));
		Setattr((yyval.node),"name",(yyvsp[-5].str));
		 Setattr((yyval.node),"storage",(yyvsp[-6].str));
		SwigType_add_function(t,(yyvsp[-2].pl));
		if ((yyvsp[0].dtype).qualifier) {
		  SwigType_push(t,(yyvsp[0].dtype).qualifier);
		}
		if ((yyvsp[0].dtype).val) {
		  Setattr((yyval.node),"value",(yyvsp[0].dtype).val);
		}
		Setattr((yyval.node),"refqualifier",(yyvsp[0].dtype).refqualifier);
		Setattr((yyval.node),"decl",t);
		Setattr((yyval.node),"parms",(yyvsp[-2].pl));
		Setattr((yyval.node),"conversion_operator","1");
		add_symbols((yyval.node));
		Delete((yyvsp[-5].str));
		Delete((yyvsp[-6].str));
              }
#line 9112 "CParse/parser.c"
    break;

  case 235: /* cpp_catch_decl: CATCH LPAREN parms RPAREN LBRACE  */
#line 5186 "./CParse/parser.y"
                                                  {
                 if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
                 (yyval.node) = 0;
               }
#line 9121 "CParse/parser.c"
    break;

  case 236: /* cpp_static_assert: STATIC_ASSERT LPAREN  */
#line 5194 "./CParse/parser.y"
                                         {
                if (skip_balanced('(',')') < 0) Exit(EXIT_FAILURE);
                (yyval.node) = 0;
              }
#line 9130 "CParse/parser.c"
    break;

  case 237: /* cpp_protection_decl: PUBLIC COLON  */
#line 5201 "./CParse/parser.y"
                                   { 
                (yyval.node) = new_node("access");
		Setattr((yyval.node),"kind","public");
                cplus_mode = CPLUS_PUBLIC;
              }
#line 9140 "CParse/parser.c"
    break;

  case 238: /* cpp_protection_decl: PRIVATE COLON  */
#line 5208 "./CParse/parser.y"
                              { 
                (yyval.node) = new_node("access");
                Setattr((yyval.node),"kind","private");
		cplus_mode = CPLUS_PRIVATE;
	      }
#line 9150 "CParse/parser.c"
    break;

  case 239: /* cpp_protection_decl: PROTECTED COLON  */
#line 5216 "./CParse/parser.y"
                                { 
		(yyval.node) = new_node("access");
		Setattr((yyval.node),"kind","protected");
		cplus_mode = CPLUS_PROTECTED;
	      }
#line 9160 "CParse/parser.c"
    break;

  case 250: /* cpp_vend: cpp_const SEMI  */
#line 5239 "./CParse/parser.y"
                                { 
                     Clear(scanner_ccode);
                     (yyval.dtype) = (yyvsp[-1].dtype);
               }
#line 9169 "CParse/parser.c"
    break;

  case 251: /* cpp_vend: cpp_const EQUAL definetype SEMI  */
#line 5243 "./CParse/parser.y"
                                                 { 
                     Clear(scanner_ccode);
                     (yyval.dtype) = (yyvsp[-3].dtype);
                     (yyval.dtype).val = (yyvsp[-1].dtype).val;
               }
#line 9179 "CParse/parser.c"
    break;

  case 252: /* cpp_vend: cpp_const LBRACE  */
#line 5248 "./CParse/parser.y"
                                  { 
                     if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
                     (yyval.dtype) = (yyvsp[-1].dtype);
               }
#line 9188 "CParse/parser.c"
    break;

  case 253: /* anonymous_bitfield: storage_class anon_bitfield_type COLON expr SEMI  */
#line 5255 "./CParse/parser.y"
                                                                       { Delete((yyvsp[-4].str)); }
#line 9194 "CParse/parser.c"
    break;

  case 258: /* anon_bitfield_type: idcolon  */
#line 5263 "./CParse/parser.y"
                         { (yyval.type) = (yyvsp[0].str); }
#line 9200 "CParse/parser.c"
    break;

  case 259: /* storage_class: storage_class_list  */
#line 5269 "./CParse/parser.y"
                                    {
		 String *r = NewStringEmpty();

		 /* Check for invalid combinations. */
		 if (multiple_bits_set((yyvsp[0].intvalue) & (SWIG_STORAGE_CLASS_EXTERN |
					     SWIG_STORAGE_CLASS_STATIC))) {
		   Swig_error(cparse_file, cparse_line, "Storage class can't be both 'static' and 'extern'");
		 }
		 if (multiple_bits_set((yyvsp[0].intvalue) & (SWIG_STORAGE_CLASS_EXTERNC |
					     SWIG_STORAGE_CLASS_EXTERN |
					     SWIG_STORAGE_CLASS_EXTERNCPP))) {
		   Swig_error(cparse_file, cparse_line, "Declaration can only be one of 'extern', 'extern \"C\"' and 'extern \"C++\"'");
		 }

		 if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_TYPEDEF) {
		   Append(r, "typedef ");
		 } else {
		   if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_EXTERNC)
		     Append(r, "externc ");
		   if ((yyvsp[0].intvalue) & (SWIG_STORAGE_CLASS_EXTERN|SWIG_STORAGE_CLASS_EXTERNCPP))
		     Append(r, "extern ");
		   if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_STATIC)
		     Append(r, "static ");
		 }
		 if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_VIRTUAL)
		   Append(r, "virtual ");
		 if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_FRIEND)
		   Append(r, "friend ");
		 if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_EXPLICIT)
		   Append(r, "explicit ");
		 if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_CONSTEXPR)
		   Append(r, "constexpr ");
		 if ((yyvsp[0].intvalue) & SWIG_STORAGE_CLASS_THREAD_LOCAL)
		   Append(r, "thread_local ");
		 if (Len(r) == 0) {
		   Delete(r);
		   (yyval.str) = 0;
		 } else {
		   Chop(r);
		   (yyval.str) = r;
		 }
	       }
#line 9247 "CParse/parser.c"
    break;

  case 260: /* storage_class: %empty  */
#line 5311 "./CParse/parser.y"
                        { (yyval.str) = 0; }
#line 9253 "CParse/parser.c"
    break;

  case 262: /* storage_class_list: storage_class_list storage_class_raw  */
#line 5315 "./CParse/parser.y"
                                                          {
		  if ((yyvsp[-1].intvalue) & (yyvsp[0].intvalue)) {
		    Swig_error(cparse_file, cparse_line, "Repeated storage class or type specifier '%s'\n", storage_class_string((yyvsp[0].intvalue)));
		  }
		  (yyval.intvalue) = (yyvsp[-1].intvalue) | (yyvsp[0].intvalue);
	       }
#line 9264 "CParse/parser.c"
    break;

  case 263: /* storage_class_raw: EXTERN  */
#line 5323 "./CParse/parser.y"
                            { (yyval.intvalue) = SWIG_STORAGE_CLASS_EXTERN; }
#line 9270 "CParse/parser.c"
    break;

  case 264: /* storage_class_raw: EXTERN string  */
#line 5324 "./CParse/parser.y"
                               {
		   if (Strcmp((yyvsp[0].str),"C") == 0) {
		     (yyval.intvalue) = SWIG_STORAGE_CLASS_EXTERNC;
		   } else if (Strcmp((yyvsp[0].str),"C++") == 0) {
		     (yyval.intvalue) = SWIG_STORAGE_CLASS_EXTERNCPP;
		   } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\".\n", (yyvsp[0].str));
		     (yyval.intvalue) = 0;
		   }
	       }
#line 9285 "CParse/parser.c"
    break;

  case 265: /* storage_class_raw: STATIC  */
#line 5334 "./CParse/parser.y"
                        { (yyval.intvalue) = SWIG_STORAGE_CLASS_STATIC; }
#line 9291 "CParse/parser.c"
    break;

  case 266: /* storage_class_raw: TYPEDEF  */
#line 5335 "./CParse/parser.y"
                         { (yyval.intvalue) = SWIG_STORAGE_CLASS_TYPEDEF; }
#line 9297 "CParse/parser.c"
    break;

  case 267: /* storage_class_raw: VIRTUAL  */
#line 5336 "./CParse/parser.y"
                         { (yyval.intvalue) = SWIG_STORAGE_CLASS_VIRTUAL; }
#line 9303 "CParse/parser.c"
    break;

  case 268: /* storage_class_raw: FRIEND  */
#line 5337 "./CParse/parser.y"
                        { (yyval.intvalue) = SWIG_STORAGE_CLASS_FRIEND; }
#line 9309 "CParse/parser.c"
    break;

  case 269: /* storage_class_raw: EXPLICIT  */
#line 5338 "./CParse/parser.y"
                          { (yyval.intvalue) = SWIG_STORAGE_CLASS_EXPLICIT; }
#line 9315 "CParse/parser.c"
    break;

  case 270: /* storage_class_raw: CONSTEXPR  */
#line 5339 "./CParse/parser.y"
                           { (yyval.intvalue) = SWIG_STORAGE_CLASS_CONSTEXPR; }
#line 9321 "CParse/parser.c"
    break;

  case 271: /* storage_class_raw: THREAD_LOCAL  */
#line 5340 "./CParse/parser.y"
                              { (yyval.intvalue) = SWIG_STORAGE_CLASS_THREAD_LOCAL; }
#line 9327 "CParse/parser.c"
    break;

  case 272: /* parms: rawparms  */
#line 5347 "./CParse/parser.y"
                          {
                 Parm *p;
		 (yyval.pl) = (yyvsp[0].pl);
		 p = (yyvsp[0].pl);
                 while (p) {
		   Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   p = nextSibling(p);
                 }
               }
#line 9341 "CParse/parser.c"
    break;

  case 273: /* rawparms: parm  */
#line 5359 "./CParse/parser.y"
                       { (yyval.pl) = (yyvsp[0].p); }
#line 9347 "CParse/parser.c"
    break;

  case 274: /* rawparms: parm DOXYGENPOSTSTRING  */
#line 5360 "./CParse/parser.y"
                                         {
		  set_comment((yyvsp[-1].p), (yyvsp[0].str));
		  (yyval.pl) = (yyvsp[-1].p);
		}
#line 9356 "CParse/parser.c"
    break;

  case 275: /* rawparms: parm DOXYGENSTRING  */
#line 5364 "./CParse/parser.y"
                                     {
		  /* Misplaced doxygen string, attach it to previous parameter, like Doxygen does */
		  set_comment((yyvsp[-1].p), (yyvsp[0].str));
		  (yyval.pl) = (yyvsp[-1].p);
		}
#line 9366 "CParse/parser.c"
    break;

  case 276: /* rawparms: parm COMMA parms  */
#line 5369 "./CParse/parser.y"
                                   {
		  if ((yyvsp[0].pl)) {
		    set_nextSibling((yyvsp[-2].p), (yyvsp[0].pl));
		  }
		  (yyval.pl) = (yyvsp[-2].p);
		}
#line 9377 "CParse/parser.c"
    break;

  case 277: /* rawparms: parm DOXYGENPOSTSTRING COMMA parms  */
#line 5375 "./CParse/parser.y"
                                                     {
		  if ((yyvsp[0].pl)) {
		    set_nextSibling((yyvsp[-3].p), (yyvsp[0].pl));
		  }
		  set_comment((yyvsp[-3].p), (yyvsp[-2].str));
		  (yyval.pl) = (yyvsp[-3].p);
		}
#line 9389 "CParse/parser.c"
    break;

  case 278: /* rawparms: parm COMMA DOXYGENPOSTSTRING parms  */
#line 5382 "./CParse/parser.y"
                                                     {
		  if ((yyvsp[0].pl)) {
		    set_nextSibling((yyvsp[-3].p), (yyvsp[0].pl));
		  }
		  set_comment((yyvsp[-3].p), (yyvsp[-1].str));
		  (yyval.pl) = (yyvsp[-3].p);
		}
#line 9401 "CParse/parser.c"
    break;

  case 279: /* rawparms: %empty  */
#line 5389 "./CParse/parser.y"
                         {
		  (yyval.pl) = 0;
		}
#line 9409 "CParse/parser.c"
    break;

  case 280: /* parm_no_dox: rawtype parameter_declarator  */
#line 5394 "./CParse/parser.y"
                                               {
                   SwigType_push((yyvsp[-1].type),(yyvsp[0].decl).type);
		   (yyval.p) = NewParmWithoutFileLineInfo((yyvsp[-1].type),(yyvsp[0].decl).id);
		   Setfile((yyval.p),cparse_file);
		   Setline((yyval.p),cparse_line);
		   if ((yyvsp[0].decl).defarg)
		     Setattr((yyval.p), "value", (yyvsp[0].decl).defarg);
		   if ((yyvsp[0].decl).stringdefarg)
		     Setattr((yyval.p), "stringval", (yyvsp[0].decl).stringdefarg);
		   if ((yyvsp[0].decl).numdefarg)
		     Setattr((yyval.p), "numval", (yyvsp[0].decl).numdefarg);
		}
#line 9426 "CParse/parser.c"
    break;

  case 281: /* parm_no_dox: ELLIPSIS  */
#line 5406 "./CParse/parser.y"
                           {
		  SwigType *t = NewString("v(...)");
		  (yyval.p) = NewParmWithoutFileLineInfo(t, 0);
		  Setfile((yyval.p),cparse_file);
		  Setline((yyval.p),cparse_line);
		}
#line 9437 "CParse/parser.c"
    break;

  case 283: /* parm: DOXYGENSTRING parm_no_dox  */
#line 5415 "./CParse/parser.y"
                                            {
		  (yyval.p) = (yyvsp[0].p);
		  set_comment((yyvsp[0].p), (yyvsp[-1].str));
		}
#line 9446 "CParse/parser.c"
    break;

  case 284: /* valparms: valparms_builder  */
#line 5421 "./CParse/parser.y"
                            {
		 (yyval.p) = (yyvsp[0].pbuilder).parms;
                 for (Parm *p = (yyval.p); p; p = nextSibling(p)) {
		   if (Getattr(p,"type")) {
		     Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   }
                 }
	       }
#line 9459 "CParse/parser.c"
    break;

  case 285: /* valparms: %empty  */
#line 5429 "./CParse/parser.y"
                        {
		 (yyval.p) = 0;
	       }
#line 9467 "CParse/parser.c"
    break;

  case 286: /* valparms_builder: valparm  */
#line 5434 "./CParse/parser.y"
                           {
		    (yyval.pbuilder).parms = (yyval.pbuilder).last = (yyvsp[0].p);
		  }
#line 9475 "CParse/parser.c"
    break;

  case 287: /* valparms_builder: valparms_builder COMMA valparm  */
#line 5437 "./CParse/parser.y"
                                                       {
		    // Build a linked list in the order specified, but avoiding
		    // a right recursion rule because "Right recursion uses up
		    // space on the Bison stack in proportion to the number of
		    // elements in the sequence".
		    set_nextSibling((yyvsp[-2].pbuilder).last, (yyvsp[0].p));
		    (yyval.pbuilder).parms = (yyvsp[-2].pbuilder).parms;
		    (yyval.pbuilder).last = (yyvsp[0].p);
		  }
#line 9489 "CParse/parser.c"
    break;

  case 288: /* valparm: parm  */
#line 5448 "./CParse/parser.y"
                      {
		  (yyval.p) = (yyvsp[0].p);
		  {
		    /* We need to make a possible adjustment for integer parameters. */
		    SwigType *type;
		    Node     *n = 0;

		    while (!n) {
		      type = Getattr((yyvsp[0].p),"type");
		      n = Swig_symbol_clookup(type,0);     /* See if we can find a node that matches the typename */
		      if ((n) && (Strcmp(nodeType(n),"cdecl") == 0)) {
			SwigType *decl = Getattr(n,"decl");
			if (!SwigType_isfunction(decl)) {
			  String *value = Getattr(n,"value");
			  if (value) {
			    String *v = Copy(value);
			    Setattr((yyvsp[0].p),"type",v);
			    Delete(v);
			    n = 0;
			  }
			}
		      } else {
			break;
		      }
		    }
		  }

               }
#line 9522 "CParse/parser.c"
    break;

  case 289: /* valparm: valexpr  */
#line 5476 "./CParse/parser.y"
                         {
                  (yyval.p) = NewParmWithoutFileLineInfo(0,0);
                  Setfile((yyval.p),cparse_file);
		  Setline((yyval.p),cparse_line);
		  Setattr((yyval.p),"value",(yyvsp[0].dtype).val);
		  if ((yyvsp[0].dtype).stringval) Setattr((yyval.p), "stringval", (yyvsp[0].dtype).stringval);
		  if ((yyvsp[0].dtype).numval) Setattr((yyval.p), "numval", (yyvsp[0].dtype).numval);
               }
#line 9535 "CParse/parser.c"
    break;

  case 290: /* def_args: EQUAL definetype  */
#line 5486 "./CParse/parser.y"
                                  { 
                 (yyval.dtype) = (yyvsp[0].dtype);
               }
#line 9543 "CParse/parser.c"
    break;

  case 291: /* def_args: EQUAL definetype LBRACKET  */
#line 5489 "./CParse/parser.y"
                                           {
		 if (skip_balanced('[', ']') < 0) Exit(EXIT_FAILURE);
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).type = T_UNKNOWN;
		 (yyval.dtype).val = (yyvsp[-1].dtype).val;
		 Append((yyval.dtype).val, scanner_ccode);
		 Clear(scanner_ccode);
               }
#line 9556 "CParse/parser.c"
    break;

  case 292: /* def_args: EQUAL LBRACE  */
#line 5497 "./CParse/parser.y"
                              {
		 if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewString(scanner_ccode);
		 (yyval.dtype).type = T_UNKNOWN;
	       }
#line 9567 "CParse/parser.c"
    break;

  case 293: /* def_args: %empty  */
#line 5503 "./CParse/parser.y"
                        {
		 (yyval.dtype) = default_dtype;
                 (yyval.dtype).type = T_UNKNOWN;
               }
#line 9576 "CParse/parser.c"
    break;

  case 294: /* parameter_declarator: declarator def_args  */
#line 5509 "./CParse/parser.y"
                                           {
                 (yyval.decl) = (yyvsp[-1].decl);
		 (yyval.decl).defarg = (yyvsp[0].dtype).val;
		 (yyval.decl).stringdefarg = (yyvsp[0].dtype).stringval;
		 (yyval.decl).numdefarg = (yyvsp[0].dtype).numval;
            }
#line 9587 "CParse/parser.c"
    break;

  case 295: /* parameter_declarator: abstract_declarator def_args  */
#line 5515 "./CParse/parser.y"
                                           {
	      (yyval.decl) = (yyvsp[-1].decl);
	      (yyval.decl).defarg = (yyvsp[0].dtype).val;
	      (yyval.decl).stringdefarg = (yyvsp[0].dtype).stringval;
	      (yyval.decl).numdefarg = (yyvsp[0].dtype).numval;
            }
#line 9598 "CParse/parser.c"
    break;

  case 296: /* parameter_declarator: def_args  */
#line 5521 "./CParse/parser.y"
                       {
	      (yyval.decl) = default_decl;
	      (yyval.decl).defarg = (yyvsp[0].dtype).val;
	      (yyval.decl).stringdefarg = (yyvsp[0].dtype).stringval;
	      (yyval.decl).numdefarg = (yyvsp[0].dtype).numval;
            }
#line 9609 "CParse/parser.c"
    break;

  case 297: /* parameter_declarator: direct_declarator LPAREN parms RPAREN qualifiers_exception_specification  */
#line 5529 "./CParse/parser.y"
                                                                                       {
	      SwigType *t;
	      (yyval.decl) = (yyvsp[-4].decl);
	      t = NewStringEmpty();
	      SwigType_add_function(t,(yyvsp[-2].pl));
	      if ((yyvsp[0].dtype).qualifier)
		SwigType_push(t, (yyvsp[0].dtype).qualifier);
	      if ((yyvsp[0].dtype).nexcept)
		SwigType_add_qualifier(t, "noexcept");
	      if (!(yyval.decl).have_parms) {
		(yyval.decl).parms = (yyvsp[-2].pl);
		(yyval.decl).have_parms = 1;
	      }
	      if (!(yyval.decl).type) {
		(yyval.decl).type = t;
	      } else {
		SwigType_push(t, (yyval.decl).type);
		Delete((yyval.decl).type);
		(yyval.decl).type = t;
	      }
	    }
#line 9635 "CParse/parser.c"
    break;

  case 298: /* plain_declarator: declarator  */
#line 5552 "./CParse/parser.y"
                              {
                 (yyval.decl) = (yyvsp[0].decl);
		 if (SwigType_isfunction((yyvsp[0].decl).type)) {
		   Delete(SwigType_pop_function((yyvsp[0].decl).type));
		 } else if (SwigType_isarray((yyvsp[0].decl).type)) {
		   SwigType *ta = SwigType_pop_arrays((yyvsp[0].decl).type);
		   if (SwigType_isfunction((yyvsp[0].decl).type)) {
		     Delete(SwigType_pop_function((yyvsp[0].decl).type));
		   } else {
		     (yyval.decl).parms = 0;
		   }
		   SwigType_push((yyvsp[0].decl).type,ta);
		   Delete(ta);
		 } else {
		   (yyval.decl).parms = 0;
		 }
            }
#line 9657 "CParse/parser.c"
    break;

  case 299: /* plain_declarator: abstract_declarator  */
#line 5569 "./CParse/parser.y"
                                  {
              (yyval.decl) = (yyvsp[0].decl);
	      if (SwigType_isfunction((yyvsp[0].decl).type)) {
		Delete(SwigType_pop_function((yyvsp[0].decl).type));
	      } else if (SwigType_isarray((yyvsp[0].decl).type)) {
		SwigType *ta = SwigType_pop_arrays((yyvsp[0].decl).type);
		if (SwigType_isfunction((yyvsp[0].decl).type)) {
		  Delete(SwigType_pop_function((yyvsp[0].decl).type));
		} else {
		  (yyval.decl).parms = 0;
		}
		SwigType_push((yyvsp[0].decl).type,ta);
		Delete(ta);
	      } else {
		(yyval.decl).parms = 0;
	      }
            }
#line 9679 "CParse/parser.c"
    break;

  case 300: /* plain_declarator: direct_declarator LPAREN parms RPAREN cv_ref_qualifier  */
#line 5588 "./CParse/parser.y"
                                                                     {
	      SwigType *t;
	      (yyval.decl) = (yyvsp[-4].decl);
	      t = NewStringEmpty();
	      SwigType_add_function(t, (yyvsp[-2].pl));
	      if ((yyvsp[0].dtype).qualifier)
	        SwigType_push(t, (yyvsp[0].dtype).qualifier);
	      if (!(yyval.decl).have_parms) {
		(yyval.decl).parms = (yyvsp[-2].pl);
		(yyval.decl).have_parms = 1;
	      }
	      if (!(yyval.decl).type) {
		(yyval.decl).type = t;
	      } else {
		SwigType_push(t, (yyval.decl).type);
		Delete((yyval.decl).type);
		(yyval.decl).type = t;
	      }
	    }
#line 9703 "CParse/parser.c"
    break;

  case 301: /* plain_declarator: %empty  */
#line 5607 "./CParse/parser.y"
                     {
	      (yyval.decl) = default_decl;
	    }
#line 9711 "CParse/parser.c"
    break;

  case 302: /* declarator: pointer notso_direct_declarator  */
#line 5612 "./CParse/parser.y"
                                              {
              (yyval.decl) = (yyvsp[0].decl);
	      if ((yyval.decl).type) {
		SwigType_push((yyvsp[-1].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[-1].type);
           }
#line 9724 "CParse/parser.c"
    break;

  case 303: /* declarator: pointer AND notso_direct_declarator  */
#line 5620 "./CParse/parser.y"
                                                 {
              (yyval.decl) = (yyvsp[0].decl);
	      SwigType_add_reference((yyvsp[-2].type));
              if ((yyval.decl).type) {
		SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[-2].type);
           }
#line 9738 "CParse/parser.c"
    break;

  case 304: /* declarator: pointer LAND notso_direct_declarator  */
#line 5629 "./CParse/parser.y"
                                                  {
              (yyval.decl) = (yyvsp[0].decl);
	      SwigType_add_rvalue_reference((yyvsp[-2].type));
              if ((yyval.decl).type) {
		SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[-2].type);
           }
#line 9752 "CParse/parser.c"
    break;

  case 305: /* declarator: direct_declarator  */
#line 5638 "./CParse/parser.y"
                               {
              (yyval.decl) = (yyvsp[0].decl);
	      if (!(yyval.decl).type) (yyval.decl).type = NewStringEmpty();
           }
#line 9761 "CParse/parser.c"
    break;

  case 306: /* declarator: AND notso_direct_declarator  */
#line 5642 "./CParse/parser.y"
                                         {
	     (yyval.decl) = (yyvsp[0].decl);
	     (yyval.decl).type = NewStringEmpty();
	     SwigType_add_reference((yyval.decl).type);
	     if ((yyvsp[0].decl).type) {
	       SwigType_push((yyval.decl).type,(yyvsp[0].decl).type);
	       Delete((yyvsp[0].decl).type);
	     }
           }
#line 9775 "CParse/parser.c"
    break;

  case 307: /* declarator: LAND notso_direct_declarator  */
#line 5651 "./CParse/parser.y"
                                          {
	     /* Introduced in C++11, move operator && */
             /* Adds one S/R conflict */
	     (yyval.decl) = (yyvsp[0].decl);
	     (yyval.decl).type = NewStringEmpty();
	     SwigType_add_rvalue_reference((yyval.decl).type);
	     if ((yyvsp[0].decl).type) {
	       SwigType_push((yyval.decl).type,(yyvsp[0].decl).type);
	       Delete((yyvsp[0].decl).type);
	     }
           }
#line 9791 "CParse/parser.c"
    break;

  case 308: /* declarator: idcolon DSTAR notso_direct_declarator  */
#line 5662 "./CParse/parser.y"
                                                   { 
	     SwigType *t = NewStringEmpty();

	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-2].str));
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = t;
	     }
#line 9807 "CParse/parser.c"
    break;

  case 309: /* declarator: pointer idcolon DSTAR notso_direct_declarator  */
#line 5673 "./CParse/parser.y"
                                                           { 
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-2].str));
	     SwigType_push((yyvsp[-3].type),t);
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[-3].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[-3].type);
	     Delete(t);
	   }
#line 9824 "CParse/parser.c"
    break;

  case 310: /* declarator: pointer idcolon DSTAR AND notso_direct_declarator  */
#line 5685 "./CParse/parser.y"
                                                               { 
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer((yyvsp[-4].type),(yyvsp[-3].str));
	     SwigType_add_reference((yyvsp[-4].type));
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[-4].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[-4].type);
	   }
#line 9839 "CParse/parser.c"
    break;

  case 311: /* declarator: idcolon DSTAR AND notso_direct_declarator  */
#line 5695 "./CParse/parser.y"
                                                       { 
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-3].str));
	     SwigType_add_reference(t);
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     } 
	     (yyval.decl).type = t;
	   }
#line 9855 "CParse/parser.c"
    break;

  case 312: /* declarator: pointer ELLIPSIS notso_direct_declarator  */
#line 5709 "./CParse/parser.y"
                                                       {
              (yyval.decl) = (yyvsp[0].decl);
	      if ((yyval.decl).type) {
		SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[-2].type);
	      SwigType_add_variadic((yyval.decl).type);
           }
#line 9869 "CParse/parser.c"
    break;

  case 313: /* declarator: pointer AND ELLIPSIS notso_direct_declarator  */
#line 5718 "./CParse/parser.y"
                                                          {
              (yyval.decl) = (yyvsp[0].decl);
	      SwigType_add_reference((yyvsp[-3].type));
              if ((yyval.decl).type) {
		SwigType_push((yyvsp[-3].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[-3].type);
	      SwigType_add_variadic((yyval.decl).type);
           }
#line 9884 "CParse/parser.c"
    break;

  case 314: /* declarator: pointer LAND ELLIPSIS notso_direct_declarator  */
#line 5728 "./CParse/parser.y"
                                                           {
              (yyval.decl) = (yyvsp[0].decl);
	      SwigType_add_rvalue_reference((yyvsp[-3].type));
              if ((yyval.decl).type) {
		SwigType_push((yyvsp[-3].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[-3].type);
	      SwigType_add_variadic((yyval.decl).type);
           }
#line 9899 "CParse/parser.c"
    break;

  case 315: /* declarator: AND ELLIPSIS notso_direct_declarator  */
#line 5738 "./CParse/parser.y"
                                                  {
	     (yyval.decl) = (yyvsp[0].decl);
	     (yyval.decl).type = NewStringEmpty();
	     SwigType_add_reference((yyval.decl).type);
	     SwigType_add_variadic((yyval.decl).type);
	     if ((yyvsp[0].decl).type) {
	       SwigType_push((yyval.decl).type,(yyvsp[0].decl).type);
	       Delete((yyvsp[0].decl).type);
	     }
           }
#line 9914 "CParse/parser.c"
    break;

  case 316: /* declarator: LAND ELLIPSIS notso_direct_declarator  */
#line 5748 "./CParse/parser.y"
                                                   {
	     /* Introduced in C++11, move operator && */
             /* Adds one S/R conflict */
	     (yyval.decl) = (yyvsp[0].decl);
	     (yyval.decl).type = NewStringEmpty();
	     SwigType_add_rvalue_reference((yyval.decl).type);
	     SwigType_add_variadic((yyval.decl).type);
	     if ((yyvsp[0].decl).type) {
	       SwigType_push((yyval.decl).type,(yyvsp[0].decl).type);
	       Delete((yyvsp[0].decl).type);
	     }
           }
#line 9931 "CParse/parser.c"
    break;

  case 317: /* declarator: idcolon DSTAR ELLIPSIS notso_direct_declarator  */
#line 5760 "./CParse/parser.y"
                                                            {
	     SwigType *t = NewStringEmpty();

	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-3].str));
	     SwigType_add_variadic(t);
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = t;
	     }
#line 9948 "CParse/parser.c"
    break;

  case 318: /* declarator: pointer idcolon DSTAR ELLIPSIS notso_direct_declarator  */
#line 5772 "./CParse/parser.y"
                                                                    {
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-3].str));
	     SwigType_add_variadic(t);
	     SwigType_push((yyvsp[-4].type),t);
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[-4].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[-4].type);
	     Delete(t);
	   }
#line 9966 "CParse/parser.c"
    break;

  case 319: /* declarator: pointer idcolon DSTAR AND ELLIPSIS notso_direct_declarator  */
#line 5785 "./CParse/parser.y"
                                                                        {
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer((yyvsp[-5].type),(yyvsp[-4].str));
	     SwigType_add_reference((yyvsp[-5].type));
	     SwigType_add_variadic((yyvsp[-5].type));
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[-5].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[-5].type);
	   }
#line 9982 "CParse/parser.c"
    break;

  case 320: /* declarator: pointer idcolon DSTAR LAND ELLIPSIS notso_direct_declarator  */
#line 5796 "./CParse/parser.y"
                                                                         {
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer((yyvsp[-5].type),(yyvsp[-4].str));
	     SwigType_add_rvalue_reference((yyvsp[-5].type));
	     SwigType_add_variadic((yyvsp[-5].type));
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[-5].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[-5].type);
	   }
#line 9998 "CParse/parser.c"
    break;

  case 321: /* declarator: idcolon DSTAR AND ELLIPSIS notso_direct_declarator  */
#line 5807 "./CParse/parser.y"
                                                                {
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-4].str));
	     SwigType_add_reference(t);
	     SwigType_add_variadic(t);
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     } 
	     (yyval.decl).type = t;
	   }
#line 10015 "CParse/parser.c"
    break;

  case 322: /* declarator: idcolon DSTAR LAND ELLIPSIS notso_direct_declarator  */
#line 5819 "./CParse/parser.y"
                                                                 {
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[0].decl);
	     SwigType_add_memberpointer(t,(yyvsp[-4].str));
	     SwigType_add_rvalue_reference(t);
	     SwigType_add_variadic(t);
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     } 
	     (yyval.decl).type = t;
	   }
#line 10032 "CParse/parser.c"
    break;

  case 323: /* notso_direct_declarator: idcolon  */
#line 5833 "./CParse/parser.y"
                                  {
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
		 (yyval.decl) = default_decl;
                 (yyval.decl).id = Char((yyvsp[0].str));
                  }
#line 10042 "CParse/parser.c"
    break;

  case 324: /* notso_direct_declarator: NOT idcolon  */
#line 5838 "./CParse/parser.y"
                                {
		  (yyval.decl) = default_decl;
                  (yyval.decl).id = Char(NewStringf("~%s",(yyvsp[0].str)));
                  }
#line 10051 "CParse/parser.c"
    break;

  case 325: /* notso_direct_declarator: LPAREN idcolon RPAREN  */
#line 5844 "./CParse/parser.y"
                                         {
		  (yyval.decl) = default_decl;
                  (yyval.decl).id = Char((yyvsp[-1].str));
                  }
#line 10060 "CParse/parser.c"
    break;

  case 326: /* notso_direct_declarator: LPAREN pointer notso_direct_declarator RPAREN  */
#line 5856 "./CParse/parser.y"
                                                                      {
		    (yyval.decl) = (yyvsp[-1].decl);
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[-2].type);
                  }
#line 10073 "CParse/parser.c"
    break;

  case 327: /* notso_direct_declarator: LPAREN idcolon DSTAR notso_direct_declarator RPAREN  */
#line 5864 "./CParse/parser.y"
                                                                            {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-1].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t,(yyvsp[-3].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		    }
#line 10089 "CParse/parser.c"
    break;

  case 328: /* notso_direct_declarator: notso_direct_declarator LBRACKET RBRACKET  */
#line 5875 "./CParse/parser.y"
                                                                  { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-2].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,"");
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
#line 10105 "CParse/parser.c"
    break;

  case 329: /* notso_direct_declarator: notso_direct_declarator LBRACKET expr RBRACKET  */
#line 5886 "./CParse/parser.y"
                                                                       { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-3].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(yyvsp[-1].dtype).val);
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
#line 10121 "CParse/parser.c"
    break;

  case 330: /* notso_direct_declarator: notso_direct_declarator LPAREN parms RPAREN  */
#line 5897 "./CParse/parser.y"
                                                                    {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[-3].decl);
		    t = NewStringEmpty();
		    SwigType_add_function(t,(yyvsp[-1].pl));
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[-1].pl);
		      (yyval.decl).have_parms = 1;
		    }
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		  }
#line 10143 "CParse/parser.c"
    break;

  case 331: /* direct_declarator: idcolon  */
#line 5916 "./CParse/parser.y"
                            {
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
		 (yyval.decl) = default_decl;
                 (yyval.decl).id = Char((yyvsp[0].str));
                  }
#line 10153 "CParse/parser.c"
    break;

  case 332: /* direct_declarator: NOT idcolon  */
#line 5922 "./CParse/parser.y"
                                {
		  (yyval.decl) = default_decl;
                  (yyval.decl).id = Char(NewStringf("~%s",(yyvsp[0].str)));
                  }
#line 10162 "CParse/parser.c"
    break;

  case 333: /* direct_declarator: LPAREN pointer direct_declarator RPAREN  */
#line 5935 "./CParse/parser.y"
                                                                {
		    (yyval.decl) = (yyvsp[-1].decl);
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[-2].type);
                  }
#line 10175 "CParse/parser.c"
    break;

  case 334: /* direct_declarator: LPAREN AND direct_declarator RPAREN  */
#line 5943 "./CParse/parser.y"
                                                            {
                    (yyval.decl) = (yyvsp[-1].decl);
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = NewStringEmpty();
		    }
		    SwigType_add_reference((yyval.decl).type);
                  }
#line 10187 "CParse/parser.c"
    break;

  case 335: /* direct_declarator: LPAREN LAND direct_declarator RPAREN  */
#line 5950 "./CParse/parser.y"
                                                             {
                    (yyval.decl) = (yyvsp[-1].decl);
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = NewStringEmpty();
		    }
		    SwigType_add_rvalue_reference((yyval.decl).type);
                  }
#line 10199 "CParse/parser.c"
    break;

  case 336: /* direct_declarator: LPAREN idcolon DSTAR declarator RPAREN  */
#line 5957 "./CParse/parser.y"
                                                           {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-1].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t,(yyvsp[-3].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		  }
#line 10215 "CParse/parser.c"
    break;

  case 337: /* direct_declarator: LPAREN idcolon DSTAR type_qualifier declarator RPAREN  */
#line 5968 "./CParse/parser.y"
                                                                          {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-1].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t, (yyvsp[-4].str));
		    SwigType_push(t, (yyvsp[-2].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		  }
#line 10232 "CParse/parser.c"
    break;

  case 338: /* direct_declarator: LPAREN idcolon DSTAR abstract_declarator RPAREN  */
#line 5980 "./CParse/parser.y"
                                                                    {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-1].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t, (yyvsp[-3].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		  }
#line 10248 "CParse/parser.c"
    break;

  case 339: /* direct_declarator: LPAREN idcolon DSTAR type_qualifier abstract_declarator RPAREN  */
#line 5991 "./CParse/parser.y"
                                                                                   {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-1].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t, (yyvsp[-4].str));
		    SwigType_push(t, (yyvsp[-2].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		  }
#line 10265 "CParse/parser.c"
    break;

  case 340: /* direct_declarator: direct_declarator LBRACKET RBRACKET  */
#line 6003 "./CParse/parser.y"
                                                            { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-2].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,"");
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
#line 10281 "CParse/parser.c"
    break;

  case 341: /* direct_declarator: direct_declarator LBRACKET expr RBRACKET  */
#line 6014 "./CParse/parser.y"
                                                                 { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-3].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(yyvsp[-1].dtype).val);
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
#line 10297 "CParse/parser.c"
    break;

  case 342: /* direct_declarator: direct_declarator LPAREN parms RPAREN  */
#line 6025 "./CParse/parser.y"
                                                              {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[-3].decl);
		    t = NewStringEmpty();
		    SwigType_add_function(t,(yyvsp[-1].pl));
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[-1].pl);
		      (yyval.decl).have_parms = 1;
		    }
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		  }
#line 10319 "CParse/parser.c"
    break;

  case 343: /* direct_declarator: OPERATOR ID LPAREN parms RPAREN  */
#line 6045 "./CParse/parser.y"
                                                   {
		    (yyval.decl) = default_decl;
		    SwigType *t;
                    Append((yyvsp[-4].str), " "); /* intervening space is mandatory */
		    Append((yyvsp[-4].str), (yyvsp[-3].id));
		    (yyval.decl).id = Char((yyvsp[-4].str));
		    t = NewStringEmpty();
		    SwigType_add_function(t,(yyvsp[-1].pl));
		    (yyval.decl).parms = (yyvsp[-1].pl);
		    (yyval.decl).have_parms = 1;
		    (yyval.decl).type = t;
		  }
#line 10336 "CParse/parser.c"
    break;

  case 344: /* abstract_declarator: pointer variadic_opt  */
#line 6059 "./CParse/parser.y"
                                           {
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = (yyvsp[-1].type);
		    if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.decl).type);
                  }
#line 10346 "CParse/parser.c"
    break;

  case 345: /* abstract_declarator: pointer direct_abstract_declarator  */
#line 6064 "./CParse/parser.y"
                                                       { 
                     (yyval.decl) = (yyvsp[0].decl);
                     SwigType_push((yyvsp[-1].type),(yyvsp[0].decl).type);
		     (yyval.decl).type = (yyvsp[-1].type);
		     Delete((yyvsp[0].decl).type);
                  }
#line 10357 "CParse/parser.c"
    break;

  case 346: /* abstract_declarator: pointer AND variadic_opt  */
#line 6070 "./CParse/parser.y"
                                             {
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = (yyvsp[-2].type);
		    SwigType_add_reference((yyval.decl).type);
		    if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.decl).type);
		  }
#line 10368 "CParse/parser.c"
    break;

  case 347: /* abstract_declarator: pointer LAND variadic_opt  */
#line 6076 "./CParse/parser.y"
                                              {
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = (yyvsp[-2].type);
		    SwigType_add_rvalue_reference((yyval.decl).type);
		    if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.decl).type);
		  }
#line 10379 "CParse/parser.c"
    break;

  case 348: /* abstract_declarator: pointer AND direct_abstract_declarator  */
#line 6082 "./CParse/parser.y"
                                                           {
		    (yyval.decl) = (yyvsp[0].decl);
		    SwigType_add_reference((yyvsp[-2].type));
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[-2].type);
                  }
#line 10393 "CParse/parser.c"
    break;

  case 349: /* abstract_declarator: pointer LAND direct_abstract_declarator  */
#line 6091 "./CParse/parser.y"
                                                            {
		    (yyval.decl) = (yyvsp[0].decl);
		    SwigType_add_rvalue_reference((yyvsp[-2].type));
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[-2].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[-2].type);
                  }
#line 10407 "CParse/parser.c"
    break;

  case 351: /* abstract_declarator: AND direct_abstract_declarator  */
#line 6101 "./CParse/parser.y"
                                                   {
		    (yyval.decl) = (yyvsp[0].decl);
		    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_reference((yyval.decl).type);
		    if ((yyvsp[0].decl).type) {
		      SwigType_push((yyval.decl).type,(yyvsp[0].decl).type);
		      Delete((yyvsp[0].decl).type);
		    }
                  }
#line 10421 "CParse/parser.c"
    break;

  case 352: /* abstract_declarator: LAND direct_abstract_declarator  */
#line 6110 "./CParse/parser.y"
                                                    {
		    (yyval.decl) = (yyvsp[0].decl);
		    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_rvalue_reference((yyval.decl).type);
		    if ((yyvsp[0].decl).type) {
		      SwigType_push((yyval.decl).type,(yyvsp[0].decl).type);
		      Delete((yyvsp[0].decl).type);
		    }
                  }
#line 10435 "CParse/parser.c"
    break;

  case 353: /* abstract_declarator: AND variadic_opt  */
#line 6119 "./CParse/parser.y"
                                     {
		    (yyval.decl) = default_decl;
                    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_reference((yyval.decl).type);
		    if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.decl).type);
                  }
#line 10446 "CParse/parser.c"
    break;

  case 354: /* abstract_declarator: LAND variadic_opt  */
#line 6125 "./CParse/parser.y"
                                      {
		    (yyval.decl) = default_decl;
                    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_rvalue_reference((yyval.decl).type);
		    if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.decl).type);
                  }
#line 10457 "CParse/parser.c"
    break;

  case 355: /* abstract_declarator: idcolon DSTAR  */
#line 6131 "./CParse/parser.y"
                                  { 
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = NewStringEmpty();
                    SwigType_add_memberpointer((yyval.decl).type,(yyvsp[-1].str));
      	          }
#line 10467 "CParse/parser.c"
    break;

  case 356: /* abstract_declarator: idcolon DSTAR type_qualifier  */
#line 6136 "./CParse/parser.y"
                                                 {
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_memberpointer((yyval.decl).type, (yyvsp[-2].str));
		    SwigType_push((yyval.decl).type, (yyvsp[0].str));
		  }
#line 10478 "CParse/parser.c"
    break;

  case 357: /* abstract_declarator: pointer idcolon DSTAR  */
#line 6142 "./CParse/parser.y"
                                          { 
		    (yyval.decl) = default_decl;
		    SwigType *t = NewStringEmpty();
                    (yyval.decl).type = (yyvsp[-2].type);
		    SwigType_add_memberpointer(t,(yyvsp[-1].str));
		    SwigType_push((yyval.decl).type,t);
		    Delete(t);
                  }
#line 10491 "CParse/parser.c"
    break;

  case 358: /* abstract_declarator: pointer idcolon DSTAR direct_abstract_declarator  */
#line 6150 "./CParse/parser.y"
                                                                     { 
		    (yyval.decl) = (yyvsp[0].decl);
		    SwigType_add_memberpointer((yyvsp[-3].type),(yyvsp[-2].str));
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[-3].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[-3].type);
                  }
#line 10505 "CParse/parser.c"
    break;

  case 359: /* direct_abstract_declarator: direct_abstract_declarator LBRACKET RBRACKET  */
#line 6161 "./CParse/parser.y"
                                                                              { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-2].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,"");
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
#line 10521 "CParse/parser.c"
    break;

  case 360: /* direct_abstract_declarator: direct_abstract_declarator LBRACKET expr RBRACKET  */
#line 6172 "./CParse/parser.y"
                                                                          { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[-3].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(yyvsp[-1].dtype).val);
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
#line 10537 "CParse/parser.c"
    break;

  case 361: /* direct_abstract_declarator: LBRACKET RBRACKET  */
#line 6183 "./CParse/parser.y"
                                      { 
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_array((yyval.decl).type,"");
                  }
#line 10547 "CParse/parser.c"
    break;

  case 362: /* direct_abstract_declarator: LBRACKET expr RBRACKET  */
#line 6188 "./CParse/parser.y"
                                           { 
		    (yyval.decl) = default_decl;
		    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_array((yyval.decl).type,(yyvsp[-1].dtype).val);
		  }
#line 10557 "CParse/parser.c"
    break;

  case 363: /* direct_abstract_declarator: LPAREN abstract_declarator RPAREN  */
#line 6193 "./CParse/parser.y"
                                                      {
                    (yyval.decl) = (yyvsp[-1].decl);
		  }
#line 10565 "CParse/parser.c"
    break;

  case 364: /* direct_abstract_declarator: direct_abstract_declarator LPAREN parms RPAREN  */
#line 6196 "./CParse/parser.y"
                                                                       {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[-3].decl);
		    t = NewStringEmpty();
                    SwigType_add_function(t,(yyvsp[-1].pl));
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[-1].pl);
		      (yyval.decl).have_parms = 1;
		    }
		  }
#line 10587 "CParse/parser.c"
    break;

  case 365: /* direct_abstract_declarator: direct_abstract_declarator LPAREN parms RPAREN cv_ref_qualifier  */
#line 6213 "./CParse/parser.y"
                                                                                        {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[-4].decl);
		    t = NewStringEmpty();
                    SwigType_add_function(t,(yyvsp[-2].pl));
		    SwigType_push(t, (yyvsp[0].dtype).qualifier);
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[-2].pl);
		      (yyval.decl).have_parms = 1;
		    }
		  }
#line 10610 "CParse/parser.c"
    break;

  case 366: /* direct_abstract_declarator: LPAREN parms RPAREN  */
#line 6231 "./CParse/parser.y"
                                        {
		    (yyval.decl) = default_decl;
                    (yyval.decl).type = NewStringEmpty();
                    SwigType_add_function((yyval.decl).type,(yyvsp[-1].pl));
		    (yyval.decl).parms = (yyvsp[-1].pl);
		    (yyval.decl).have_parms = 1;
                  }
#line 10622 "CParse/parser.c"
    break;

  case 367: /* pointer: pointer STAR type_qualifier  */
#line 6241 "./CParse/parser.y"
                                             {
	     (yyval.type) = (yyvsp[-2].type);
             SwigType_add_pointer((yyval.type));
	     SwigType_push((yyval.type),(yyvsp[0].str));
           }
#line 10632 "CParse/parser.c"
    break;

  case 368: /* pointer: pointer STAR  */
#line 6246 "./CParse/parser.y"
                              {
	     (yyval.type) = (yyvsp[-1].type);
	     SwigType_add_pointer((yyval.type));
	   }
#line 10641 "CParse/parser.c"
    break;

  case 369: /* pointer: STAR type_qualifier  */
#line 6250 "./CParse/parser.y"
                                 {
	     (yyval.type) = NewStringEmpty();
	     SwigType_add_pointer((yyval.type));
	     SwigType_push((yyval.type),(yyvsp[0].str));
           }
#line 10651 "CParse/parser.c"
    break;

  case 370: /* pointer: STAR  */
#line 6255 "./CParse/parser.y"
                  {
	     (yyval.type) = NewStringEmpty();
	     SwigType_add_pointer((yyval.type));
           }
#line 10660 "CParse/parser.c"
    break;

  case 371: /* cv_ref_qualifier: type_qualifier  */
#line 6262 "./CParse/parser.y"
                                  {
		  (yyval.dtype).qualifier = (yyvsp[0].str);
	       }
#line 10668 "CParse/parser.c"
    break;

  case 372: /* cv_ref_qualifier: type_qualifier ref_qualifier  */
#line 6265 "./CParse/parser.y"
                                              {
		  (yyval.dtype).qualifier = (yyvsp[-1].str);
		  (yyval.dtype).refqualifier = (yyvsp[0].str);
		  SwigType_push((yyval.dtype).qualifier, (yyvsp[0].str));
	       }
#line 10678 "CParse/parser.c"
    break;

  case 373: /* cv_ref_qualifier: ref_qualifier  */
#line 6270 "./CParse/parser.y"
                               {
		  (yyval.dtype).qualifier = NewStringEmpty();
		  (yyval.dtype).refqualifier = (yyvsp[0].str);
		  SwigType_push((yyval.dtype).qualifier, (yyvsp[0].str));
	       }
#line 10688 "CParse/parser.c"
    break;

  case 374: /* ref_qualifier: AND  */
#line 6277 "./CParse/parser.y"
                    {
	          (yyval.str) = NewStringEmpty();
	          SwigType_add_reference((yyval.str));
	       }
#line 10697 "CParse/parser.c"
    break;

  case 375: /* ref_qualifier: LAND  */
#line 6281 "./CParse/parser.y"
                      {
	          (yyval.str) = NewStringEmpty();
	          SwigType_add_rvalue_reference((yyval.str));
	       }
#line 10706 "CParse/parser.c"
    break;

  case 376: /* type_qualifier: type_qualifier_raw  */
#line 6287 "./CParse/parser.y"
                                    {
	          (yyval.str) = NewStringEmpty();
	          if ((yyvsp[0].id)) SwigType_add_qualifier((yyval.str),(yyvsp[0].id));
               }
#line 10715 "CParse/parser.c"
    break;

  case 377: /* type_qualifier: type_qualifier type_qualifier_raw  */
#line 6291 "./CParse/parser.y"
                                                       {
		  (yyval.str) = (yyvsp[-1].str);
	          if ((yyvsp[0].id)) SwigType_add_qualifier((yyval.str),(yyvsp[0].id));
               }
#line 10724 "CParse/parser.c"
    break;

  case 378: /* type_qualifier_raw: CONST_QUAL  */
#line 6297 "./CParse/parser.y"
                                 { (yyval.id) = "const"; }
#line 10730 "CParse/parser.c"
    break;

  case 379: /* type_qualifier_raw: VOLATILE  */
#line 6298 "./CParse/parser.y"
                               { (yyval.id) = "volatile"; }
#line 10736 "CParse/parser.c"
    break;

  case 380: /* type_qualifier_raw: REGISTER  */
#line 6299 "./CParse/parser.y"
                               { (yyval.id) = 0; }
#line 10742 "CParse/parser.c"
    break;

  case 381: /* type: rawtype  */
#line 6305 "./CParse/parser.y"
                                    {
                   (yyval.type) = (yyvsp[0].type);
                   Replace((yyval.type),"typename ","", DOH_REPLACE_ANY);
                }
#line 10751 "CParse/parser.c"
    break;

  case 382: /* rawtype: type_qualifier type_right  */
#line 6311 "./CParse/parser.y"
                                           {
                   (yyval.type) = (yyvsp[0].type);
	           SwigType_push((yyval.type),(yyvsp[-1].str));
               }
#line 10760 "CParse/parser.c"
    break;

  case 384: /* rawtype: type_right type_qualifier  */
#line 6316 "./CParse/parser.y"
                                           {
		  (yyval.type) = (yyvsp[-1].type);
	          SwigType_push((yyval.type),(yyvsp[0].str));
	       }
#line 10769 "CParse/parser.c"
    break;

  case 385: /* rawtype: type_qualifier type_right type_qualifier  */
#line 6320 "./CParse/parser.y"
                                                                                            {
		  (yyval.type) = (yyvsp[-1].type);
	          SwigType_push((yyval.type),(yyvsp[0].str));
	          SwigType_push((yyval.type),(yyvsp[-2].str));
	       }
#line 10779 "CParse/parser.c"
    break;

  case 386: /* rawtype: rawtype ELLIPSIS  */
#line 6325 "./CParse/parser.y"
                                      {
		  (yyval.type) = (yyvsp[-1].type);
		  SwigType_add_variadic((yyval.type));
	       }
#line 10788 "CParse/parser.c"
    break;

  case 390: /* type_right: c_enum_key idcolon  */
#line 6334 "./CParse/parser.y"
                                    { (yyval.type) = NewStringf("enum %s", (yyvsp[0].str)); }
#line 10794 "CParse/parser.c"
    break;

  case 392: /* type_right: idcolon  */
#line 6337 "./CParse/parser.y"
                                   {
		  (yyval.type) = (yyvsp[0].str);
               }
#line 10802 "CParse/parser.c"
    break;

  case 393: /* type_right: cpptype idcolon  */
#line 6340 "./CParse/parser.y"
                                           {
		 (yyval.type) = NewStringf("%s %s", (yyvsp[-1].type), (yyvsp[0].str));
               }
#line 10810 "CParse/parser.c"
    break;

  case 395: /* @11: %empty  */
#line 6346 "./CParse/parser.y"
                                      {
		 (yyval.str) = get_raw_text_balanced('(', ')');
	       }
#line 10818 "CParse/parser.c"
    break;

  case 396: /* decltype: DECLTYPE LPAREN @11 decltypeexpr  */
#line 6348 "./CParse/parser.y"
                                    {
		 String *expr = (yyvsp[-1].str);
		 if ((yyvsp[0].type)) {
		   (yyval.type) = (yyvsp[0].type);
		 } else {
		   (yyval.type) = NewStringf("decltype%s", expr);
		   /* expr includes parentheses but don't include them in the warning message. */
		   Delitem(expr, 0);
		   Delitem(expr, DOH_END);
		   Swig_warning(WARN_CPP11_DECLTYPE, cparse_file, cparse_line, "Unable to deduce decltype for '%s'.\n", expr);
		 }
		 Delete(expr);
	       }
#line 10836 "CParse/parser.c"
    break;

  case 397: /* decltypeexpr: expr RPAREN  */
#line 6363 "./CParse/parser.y"
                             {
		 (yyval.type) = deduce_type(&(yyvsp[-1].dtype));
	       }
#line 10844 "CParse/parser.c"
    break;

  case 398: /* decltypeexpr: error RPAREN  */
#line 6366 "./CParse/parser.y"
                              {
		 /* Avoid a parse error if we can't parse the expression
		  * decltype() is applied to.
		  *
		  * Set $$ to 0 here to trigger the decltype rule above to
		  * issue a warning.
		  */
		 (yyval.type) = 0;
		 if (skip_balanced('(',')') < 0) Exit(EXIT_FAILURE);
		 Clear(scanner_ccode);
	       }
#line 10860 "CParse/parser.c"
    break;

  case 399: /* primitive_type: primitive_type_list  */
#line 6379 "./CParse/parser.y"
                                     {
		 String *type = (yyvsp[0].ptype).type;
		 if (!type) type = NewString("int");
		 if ((yyvsp[0].ptype).us) {
		   (yyval.type) = NewStringf("%s %s", (yyvsp[0].ptype).us, type);
		   Delete((yyvsp[0].ptype).us);
                   Delete(type);
		 } else {
                   (yyval.type) = type;
		 }
		 if (Cmp((yyval.type),"signed int") == 0) {
		   Delete((yyval.type));
		   (yyval.type) = NewString("int");
                 } else if (Cmp((yyval.type),"signed long") == 0) {
		   Delete((yyval.type));
                   (yyval.type) = NewString("long");
                 } else if (Cmp((yyval.type),"signed short") == 0) {
		   Delete((yyval.type));
		   (yyval.type) = NewString("short");
		 } else if (Cmp((yyval.type),"signed long long") == 0) {
		   Delete((yyval.type));
		   (yyval.type) = NewString("long long");
		 }
               }
#line 10889 "CParse/parser.c"
    break;

  case 401: /* primitive_type_list: type_specifier primitive_type_list  */
#line 6406 "./CParse/parser.y"
                                                        {
                    if ((yyvsp[-1].ptype).us && (yyvsp[0].ptype).us) {
		      Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", (yyvsp[0].ptype).us);
		    }
                    (yyval.ptype) = (yyvsp[0].ptype);
                    if ((yyvsp[-1].ptype).us) (yyval.ptype).us = (yyvsp[-1].ptype).us;
		    if ((yyvsp[-1].ptype).type) {
		      if (!(yyvsp[0].ptype).type) (yyval.ptype).type = (yyvsp[-1].ptype).type;
		      else {
			int err = 0;
			if ((Cmp((yyvsp[-1].ptype).type,"long") == 0)) {
			  if ((Cmp((yyvsp[0].ptype).type,"long") == 0) || (Strncmp((yyvsp[0].ptype).type,"double",6) == 0)) {
			    (yyval.ptype).type = NewStringf("long %s", (yyvsp[0].ptype).type);
			  } else if (Cmp((yyvsp[0].ptype).type,"int") == 0) {
			    (yyval.ptype).type = (yyvsp[-1].ptype).type;
			  } else {
			    err = 1;
			  }
			} else if ((Cmp((yyvsp[-1].ptype).type,"short")) == 0) {
			  if (Cmp((yyvsp[0].ptype).type,"int") == 0) {
			    (yyval.ptype).type = (yyvsp[-1].ptype).type;
			  } else {
			    err = 1;
			  }
			} else if (Cmp((yyvsp[-1].ptype).type,"int") == 0) {
			  (yyval.ptype).type = (yyvsp[0].ptype).type;
			} else if (Cmp((yyvsp[-1].ptype).type,"double") == 0) {
			  if (Cmp((yyvsp[0].ptype).type,"long") == 0) {
			    (yyval.ptype).type = NewString("long double");
			  } else if (Cmp((yyvsp[0].ptype).type,"_Complex") == 0) {
			    (yyval.ptype).type = NewString("double _Complex");
			  } else {
			    err = 1;
			  }
			} else if (Cmp((yyvsp[-1].ptype).type,"float") == 0) {
			  if (Cmp((yyvsp[0].ptype).type,"_Complex") == 0) {
			    (yyval.ptype).type = NewString("float _Complex");
			  } else {
			    err = 1;
			  }
			} else if (Cmp((yyvsp[-1].ptype).type,"_Complex") == 0) {
			  (yyval.ptype).type = NewStringf("%s _Complex", (yyvsp[0].ptype).type);
			} else {
			  err = 1;
			}
			if (err) {
			  Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", (yyvsp[-1].ptype).type);
			}
		      }
		    }
               }
#line 10945 "CParse/parser.c"
    break;

  case 402: /* type_specifier: TYPE_INT  */
#line 6460 "./CParse/parser.y"
                          { 
		    (yyval.ptype).type = NewString("int");
                    (yyval.ptype).us = 0;
               }
#line 10954 "CParse/parser.c"
    break;

  case 403: /* type_specifier: TYPE_SHORT  */
#line 6464 "./CParse/parser.y"
                            { 
                    (yyval.ptype).type = NewString("short");
                    (yyval.ptype).us = 0;
                }
#line 10963 "CParse/parser.c"
    break;

  case 404: /* type_specifier: TYPE_LONG  */
#line 6468 "./CParse/parser.y"
                           { 
                    (yyval.ptype).type = NewString("long");
                    (yyval.ptype).us = 0;
                }
#line 10972 "CParse/parser.c"
    break;

  case 405: /* type_specifier: TYPE_CHAR  */
#line 6472 "./CParse/parser.y"
                           { 
                    (yyval.ptype).type = NewString("char");
                    (yyval.ptype).us = 0;
                }
#line 10981 "CParse/parser.c"
    break;

  case 406: /* type_specifier: TYPE_WCHAR  */
#line 6476 "./CParse/parser.y"
                            { 
                    (yyval.ptype).type = NewString("wchar_t");
                    (yyval.ptype).us = 0;
                }
#line 10990 "CParse/parser.c"
    break;

  case 407: /* type_specifier: TYPE_FLOAT  */
#line 6480 "./CParse/parser.y"
                            { 
                    (yyval.ptype).type = NewString("float");
                    (yyval.ptype).us = 0;
                }
#line 10999 "CParse/parser.c"
    break;

  case 408: /* type_specifier: TYPE_DOUBLE  */
#line 6484 "./CParse/parser.y"
                             { 
                    (yyval.ptype).type = NewString("double");
                    (yyval.ptype).us = 0;
                }
#line 11008 "CParse/parser.c"
    break;

  case 409: /* type_specifier: TYPE_SIGNED  */
#line 6488 "./CParse/parser.y"
                             { 
                    (yyval.ptype).us = NewString("signed");
                    (yyval.ptype).type = 0;
                }
#line 11017 "CParse/parser.c"
    break;

  case 410: /* type_specifier: TYPE_UNSIGNED  */
#line 6492 "./CParse/parser.y"
                               { 
                    (yyval.ptype).us = NewString("unsigned");
                    (yyval.ptype).type = 0;
                }
#line 11026 "CParse/parser.c"
    break;

  case 411: /* type_specifier: TYPE_COMPLEX  */
#line 6496 "./CParse/parser.y"
                              { 
                    (yyval.ptype).type = NewString("_Complex");
                    (yyval.ptype).us = 0;
                }
#line 11035 "CParse/parser.c"
    break;

  case 412: /* type_specifier: TYPE_NON_ISO_INT8  */
#line 6500 "./CParse/parser.y"
                                   { 
                    (yyval.ptype).type = NewString("__int8");
                    (yyval.ptype).us = 0;
                }
#line 11044 "CParse/parser.c"
    break;

  case 413: /* type_specifier: TYPE_NON_ISO_INT16  */
#line 6504 "./CParse/parser.y"
                                    { 
                    (yyval.ptype).type = NewString("__int16");
                    (yyval.ptype).us = 0;
                }
#line 11053 "CParse/parser.c"
    break;

  case 414: /* type_specifier: TYPE_NON_ISO_INT32  */
#line 6508 "./CParse/parser.y"
                                    { 
                    (yyval.ptype).type = NewString("__int32");
                    (yyval.ptype).us = 0;
                }
#line 11062 "CParse/parser.c"
    break;

  case 415: /* type_specifier: TYPE_NON_ISO_INT64  */
#line 6512 "./CParse/parser.y"
                                    { 
                    (yyval.ptype).type = NewString("__int64");
                    (yyval.ptype).us = 0;
                }
#line 11071 "CParse/parser.c"
    break;

  case 420: /* deleted_definition: DELETE_KW  */
#line 6527 "./CParse/parser.y"
                               {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewString("delete");
		  (yyval.dtype).type = T_STRING;
		}
#line 11081 "CParse/parser.c"
    break;

  case 421: /* explicit_default: DEFAULT  */
#line 6535 "./CParse/parser.y"
                           {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewString("default");
		  (yyval.dtype).type = T_STRING;
		}
#line 11091 "CParse/parser.c"
    break;

  case 423: /* ename: %empty  */
#line 6545 "./CParse/parser.y"
                         { (yyval.id) = 0; }
#line 11097 "CParse/parser.c"
    break;

  case 428: /* enumlist: enumlist_item  */
#line 6564 "./CParse/parser.y"
                                {
		  Setattr((yyvsp[0].node),"_last",(yyvsp[0].node));
		  (yyval.node) = (yyvsp[0].node);
		}
#line 11106 "CParse/parser.c"
    break;

  case 429: /* enumlist: enumlist_item DOXYGENPOSTSTRING  */
#line 6568 "./CParse/parser.y"
                                                  {
		  Setattr((yyvsp[-1].node),"_last",(yyvsp[-1].node));
		  set_comment((yyvsp[-1].node), (yyvsp[0].str));
		  (yyval.node) = (yyvsp[-1].node);
		}
#line 11116 "CParse/parser.c"
    break;

  case 430: /* enumlist: enumlist_item DOXYGENSTRING  */
#line 6573 "./CParse/parser.y"
                                              {
		  Setattr((yyvsp[-1].node), "_last", (yyvsp[-1].node));
		  /* Misplaced doxygen string, attach it to previous parameter, like Doxygen does */
		  set_comment((yyvsp[-1].node), (yyvsp[0].str));
		  (yyval.node) = (yyvsp[-1].node);
		}
#line 11127 "CParse/parser.c"
    break;

  case 431: /* enumlist: enumlist_item COMMA enumlist  */
#line 6579 "./CParse/parser.y"
                                                   {
		  if ((yyvsp[0].node)) {
		    set_nextSibling((yyvsp[-2].node), (yyvsp[0].node));
		    Setattr((yyvsp[-2].node),"_last",Getattr((yyvsp[0].node),"_last"));
		    Setattr((yyvsp[0].node),"_last",NULL);
		  } else {
		    Setattr((yyvsp[-2].node),"_last",(yyvsp[-2].node));
		  }
		  (yyval.node) = (yyvsp[-2].node);
		}
#line 11142 "CParse/parser.c"
    break;

  case 432: /* enumlist: enumlist_item DOXYGENPOSTSTRING COMMA enumlist  */
#line 6589 "./CParse/parser.y"
                                                                     {
		  if ((yyvsp[0].node)) {
		    set_nextSibling((yyvsp[-3].node), (yyvsp[0].node));
		    Setattr((yyvsp[-3].node),"_last",Getattr((yyvsp[0].node),"_last"));
		    Setattr((yyvsp[0].node),"_last",NULL);
		  } else {
		    Setattr((yyvsp[-3].node),"_last",(yyvsp[-3].node));
		  }
		  set_comment((yyvsp[-3].node), (yyvsp[-2].str));
		  (yyval.node) = (yyvsp[-3].node);
		}
#line 11158 "CParse/parser.c"
    break;

  case 433: /* enumlist: enumlist_item COMMA DOXYGENPOSTSTRING enumlist  */
#line 6600 "./CParse/parser.y"
                                                                     {
		  if ((yyvsp[0].node)) {
		    set_nextSibling((yyvsp[-3].node), (yyvsp[0].node));
		    Setattr((yyvsp[-3].node),"_last",Getattr((yyvsp[0].node),"_last"));
		    Setattr((yyvsp[0].node),"_last",NULL);
		  } else {
		    Setattr((yyvsp[-3].node),"_last",(yyvsp[-3].node));
		  }
		  set_comment((yyvsp[-3].node), (yyvsp[-1].str));
		  (yyval.node) = (yyvsp[-3].node);
		}
#line 11174 "CParse/parser.c"
    break;

  case 434: /* enumlist: optional_ignored_defines  */
#line 6611 "./CParse/parser.y"
                                           {
		  (yyval.node) = 0;
		}
#line 11182 "CParse/parser.c"
    break;

  case 435: /* enumlist_item: optional_ignored_defines edecl_with_dox optional_ignored_defines  */
#line 6616 "./CParse/parser.y"
                                                                                   {
		  (yyval.node) = (yyvsp[-1].node);
		}
#line 11190 "CParse/parser.c"
    break;

  case 437: /* edecl_with_dox: DOXYGENSTRING edecl  */
#line 6622 "./CParse/parser.y"
                                      {
		  (yyval.node) = (yyvsp[0].node);
		  set_comment((yyvsp[0].node), (yyvsp[-1].str));
		}
#line 11199 "CParse/parser.c"
    break;

  case 438: /* edecl: identifier  */
#line 6628 "./CParse/parser.y"
                             {
		   SwigType *type = NewSwigType(T_INT);
		   (yyval.node) = new_node("enumitem");
		   Setattr((yyval.node),"name",(yyvsp[0].id));
		   Setattr((yyval.node),"type",type);
		   SetFlag((yyval.node),"feature:immutable");
		   Delete(type);
		 }
#line 11212 "CParse/parser.c"
    break;

  case 439: /* edecl: identifier EQUAL etype  */
#line 6636 "./CParse/parser.y"
                                          {
		   SwigType *type = NewSwigType((yyvsp[0].dtype).type == T_BOOL ? T_BOOL : ((yyvsp[0].dtype).type == T_CHAR ? T_CHAR : T_INT));
		   (yyval.node) = new_node("enumitem");
		   Setattr((yyval.node),"name",(yyvsp[-2].id));
		   Setattr((yyval.node),"type",type);
		   SetFlag((yyval.node),"feature:immutable");
		   Setattr((yyval.node),"enumvalue", (yyvsp[0].dtype).val);
		   if ((yyvsp[0].dtype).stringval) {
		     Setattr((yyval.node), "enumstringval", (yyvsp[0].dtype).stringval);
		   }
		   if ((yyvsp[0].dtype).numval) {
		     Setattr((yyval.node), "enumnumval", (yyvsp[0].dtype).numval);
		   }
		   Setattr((yyval.node),"value",(yyvsp[-2].id));
		   Delete(type);
                 }
#line 11233 "CParse/parser.c"
    break;

  case 440: /* etype: expr  */
#line 6654 "./CParse/parser.y"
                        {
                   (yyval.dtype) = (yyvsp[0].dtype);
		   /* We get T_USER here for a typedef - unfortunately we can't
		    * currently resolve typedefs at this stage of parsing. */
		   if (((yyval.dtype).type != T_INT) && ((yyval.dtype).type != T_UINT) &&
		       ((yyval.dtype).type != T_LONG) && ((yyval.dtype).type != T_ULONG) &&
		       ((yyval.dtype).type != T_LONGLONG) && ((yyval.dtype).type != T_ULONGLONG) &&
		       ((yyval.dtype).type != T_SHORT) && ((yyval.dtype).type != T_USHORT) &&
		       ((yyval.dtype).type != T_SCHAR) && ((yyval.dtype).type != T_UCHAR) &&
		       ((yyval.dtype).type != T_CHAR) && ((yyval.dtype).type != T_BOOL) &&
		       ((yyval.dtype).type != T_UNKNOWN) && ((yyval.dtype).type != T_USER)) {
		     Swig_error(cparse_file,cparse_line,"Type error. Expecting an integral type\n");
		   }
                }
#line 11252 "CParse/parser.c"
    break;

  case 442: /* expr: type  */
#line 6673 "./CParse/parser.y"
                      {
		 Node *n;
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = (yyvsp[0].type);
		 (yyval.dtype).type = T_UNKNOWN;
		 /* Check if value is in scope */
		 n = Swig_symbol_clookup((yyvsp[0].type),0);
		 if (n) {
                   /* A band-aid for enum values used in expressions. */
                   if (Strcmp(nodeType(n),"enumitem") == 0) {
                     String *q = Swig_symbol_qualified(n);
                     if (q) {
                       (yyval.dtype).val = NewStringf("%s::%s", q, Getattr(n,"name"));
		       (yyval.dtype).type = SwigType_type(Getattr(n, "type"));
                       Delete(q);
                     }
		   } else {
		     SwigType *type = Getattr(n, "type");
		     if (type) {
		       (yyval.dtype).type = SwigType_type(type);
		     }
		   }
		 }
               }
#line 11281 "CParse/parser.c"
    break;

  case 443: /* exprmem: ID ARROW ID  */
#line 6700 "./CParse/parser.y"
                                       {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s->%s", (yyvsp[-2].id), (yyvsp[0].id));
	       }
#line 11290 "CParse/parser.c"
    break;

  case 444: /* exprmem: ID ARROW ID LPAREN  */
#line 6704 "./CParse/parser.y"
                                              {
		 if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s->%s%s", (yyvsp[-3].id), (yyvsp[-1].id), scanner_ccode);
		 Clear(scanner_ccode);
	       }
#line 11301 "CParse/parser.c"
    break;

  case 445: /* exprmem: exprmem ARROW ID  */
#line 6710 "./CParse/parser.y"
                                      {
		 (yyval.dtype) = (yyvsp[-2].dtype);
		 Printf((yyval.dtype).val, "->%s", (yyvsp[0].id));
	       }
#line 11310 "CParse/parser.c"
    break;

  case 446: /* exprmem: exprmem ARROW ID LPAREN  */
#line 6714 "./CParse/parser.y"
                                             {
		 if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		 (yyval.dtype) = (yyvsp[-3].dtype);
		 Printf((yyval.dtype).val, "->%s%s", (yyvsp[-1].id), scanner_ccode);
		 Clear(scanner_ccode);
	       }
#line 11321 "CParse/parser.c"
    break;

  case 447: /* exprmem: ID PERIOD ID  */
#line 6720 "./CParse/parser.y"
                                        {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s.%s", (yyvsp[-2].id), (yyvsp[0].id));
	       }
#line 11330 "CParse/parser.c"
    break;

  case 448: /* exprmem: ID PERIOD ID LPAREN  */
#line 6724 "./CParse/parser.y"
                                               {
		 if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s.%s%s", (yyvsp[-3].id), (yyvsp[-1].id), scanner_ccode);
		 Clear(scanner_ccode);
	       }
#line 11341 "CParse/parser.c"
    break;

  case 449: /* exprmem: exprmem PERIOD ID  */
#line 6730 "./CParse/parser.y"
                                       {
		 (yyval.dtype) = (yyvsp[-2].dtype);
		 Printf((yyval.dtype).val, ".%s", (yyvsp[0].id));
	       }
#line 11350 "CParse/parser.c"
    break;

  case 450: /* exprmem: exprmem PERIOD ID LPAREN  */
#line 6734 "./CParse/parser.y"
                                              {
		 if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		 (yyval.dtype) = (yyvsp[-3].dtype);
		 Printf((yyval.dtype).val, ".%s%s", (yyvsp[-1].id), scanner_ccode);
		 Clear(scanner_ccode);
	       }
#line 11361 "CParse/parser.c"
    break;

  case 453: /* exprsimple: string  */
#line 6745 "./CParse/parser.y"
                        {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).stringval = (yyvsp[0].str);
		  (yyval.dtype).val = NewStringf("\"%(escape)s\"", (yyvsp[0].str));
		  (yyval.dtype).type = T_STRING;
	       }
#line 11372 "CParse/parser.c"
    break;

  case 454: /* exprsimple: wstring  */
#line 6751 "./CParse/parser.y"
                         {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).stringval = (yyvsp[0].str);
		  (yyval.dtype).val = NewStringf("L\"%(escape)s\"", (yyvsp[0].str));
		  (yyval.dtype).type = T_WSTRING;
	       }
#line 11383 "CParse/parser.c"
    break;

  case 455: /* exprsimple: CHARCONST  */
#line 6757 "./CParse/parser.y"
                           {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).stringval = (yyvsp[0].str);
		  (yyval.dtype).val = NewStringf("'%(escape)s'", (yyvsp[0].str));
		  (yyval.dtype).type = T_CHAR;
	       }
#line 11394 "CParse/parser.c"
    break;

  case 456: /* exprsimple: WCHARCONST  */
#line 6763 "./CParse/parser.y"
                            {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).stringval = (yyvsp[0].str);
		  (yyval.dtype).val = NewStringf("L'%(escape)s'", (yyvsp[0].str));
		  (yyval.dtype).type = T_WCHAR;
	       }
#line 11405 "CParse/parser.c"
    break;

  case 457: /* exprsimple: SIZEOF LPAREN  */
#line 6775 "./CParse/parser.y"
                               {
		  if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewStringf("sizeof%s", scanner_ccode);
		  Clear(scanner_ccode);
		  (yyval.dtype).type = T_ULONG;
               }
#line 11417 "CParse/parser.c"
    break;

  case 458: /* exprsimple: ALIGNOF LPAREN  */
#line 6783 "./CParse/parser.y"
                                {
		  if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewStringf("alignof%s", scanner_ccode);
		  Clear(scanner_ccode);
		  (yyval.dtype).type = T_ULONG;
	       }
#line 11429 "CParse/parser.c"
    break;

  case 459: /* exprsimple: NOEXCEPT LPAREN  */
#line 6791 "./CParse/parser.y"
                                 {
		  if (skip_balanced('(', ')') < 0) Exit(EXIT_FAILURE);
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewStringf("noexcept%s", scanner_ccode);
		  Clear(scanner_ccode);
		  (yyval.dtype).type = T_BOOL;
	       }
#line 11441 "CParse/parser.c"
    break;

  case 460: /* exprsimple: SIZEOF ELLIPSIS LPAREN identifier RPAREN  */
#line 6798 "./CParse/parser.y"
                                                          {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewStringf("sizeof...(%s)", (yyvsp[-1].id));
		  (yyval.dtype).type = T_ULONG;
               }
#line 11451 "CParse/parser.c"
    break;

  case 461: /* exprsimple: SIZEOF exprsimple  */
#line 6808 "./CParse/parser.y"
                                       {
		  (yyval.dtype) = default_dtype;
		  (yyval.dtype).val = NewStringf("sizeof(%s)", (yyvsp[0].dtype).val);
		  (yyval.dtype).type = T_ULONG;
	       }
#line 11461 "CParse/parser.c"
    break;

  case 464: /* valexpr: LPAREN expr RPAREN  */
#line 6819 "./CParse/parser.y"
                                                {
	            (yyval.dtype) = default_dtype;
		    (yyval.dtype).val = NewStringf("(%s)",(yyvsp[-1].dtype).val);
		    (yyval.dtype).stringval = Copy((yyvsp[-1].dtype).stringval);
		    (yyval.dtype).numval = Copy((yyvsp[-1].dtype).numval);
		    (yyval.dtype).type = (yyvsp[-1].dtype).type;
	       }
#line 11473 "CParse/parser.c"
    break;

  case 465: /* valexpr: LPAREN expr RPAREN expr  */
#line 6829 "./CParse/parser.y"
                                                              {
		 int cast_type_code = SwigType_type((yyvsp[-2].dtype).val);
		 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).unary_arg_type = 0;
		 if ((yyvsp[0].dtype).type != T_STRING) {
		   switch ((yyvsp[-2].dtype).type) {
		     case T_FLOAT:
		     case T_DOUBLE:
		     case T_LONGDOUBLE:
		     case T_FLTCPLX:
		     case T_DBLCPLX:
		       (yyval.dtype).val = NewStringf("(%s)%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val); /* SwigType_str and decimal points don't mix! */
		       break;
		     default:
		       (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[-2].dtype).val,0), (yyvsp[0].dtype).val);
		       break;
		   }
		   (yyval.dtype).stringval = 0;
		   (yyval.dtype).numval = 0;
		 }
		 /* As well as C-style casts, this grammar rule currently also
		  * matches a binary operator with a LHS in parentheses for
		  * binary operators which also have an unary form, e.g.:
		  *
		  * (6)*7
		  * (6)&7
		  * (6)+7
		  * (6)-7
		  */
		 if (cast_type_code != T_USER && cast_type_code != T_UNKNOWN) {
		   /* $lhs is definitely a type so we know this is a cast. */
		   (yyval.dtype).type = cast_type_code;
		 } else if ((yyvsp[0].dtype).type == 0 || (yyvsp[0].dtype).unary_arg_type == 0) {
		   /* Not one of the cases above, so we know this is a cast. */
		   (yyval.dtype).type = cast_type_code;
		 } else {
		   (yyval.dtype).type = promote((yyvsp[-2].dtype).type, (yyvsp[0].dtype).unary_arg_type);
		 }
 	       }
#line 11517 "CParse/parser.c"
    break;

  case 466: /* valexpr: LPAREN expr pointer RPAREN expr  */
#line 6868 "./CParse/parser.y"
                                                                      {
                 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).unary_arg_type = 0;
		 if ((yyvsp[0].dtype).type != T_STRING) {
		   SwigType_push((yyvsp[-3].dtype).val,(yyvsp[-2].type));
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[-3].dtype).val,0), (yyvsp[0].dtype).val);
		   (yyval.dtype).stringval = 0;
		   (yyval.dtype).numval = 0;
		 }
 	       }
#line 11532 "CParse/parser.c"
    break;

  case 467: /* valexpr: LPAREN expr AND RPAREN expr  */
#line 6878 "./CParse/parser.y"
                                                                  {
                 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).unary_arg_type = 0;
		 if ((yyvsp[0].dtype).type != T_STRING) {
		   SwigType_add_reference((yyvsp[-3].dtype).val);
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[-3].dtype).val,0), (yyvsp[0].dtype).val);
		   (yyval.dtype).stringval = 0;
		   (yyval.dtype).numval = 0;
		 }
 	       }
#line 11547 "CParse/parser.c"
    break;

  case 468: /* valexpr: LPAREN expr LAND RPAREN expr  */
#line 6888 "./CParse/parser.y"
                                                                   {
                 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).unary_arg_type = 0;
		 if ((yyvsp[0].dtype).type != T_STRING) {
		   SwigType_add_rvalue_reference((yyvsp[-3].dtype).val);
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[-3].dtype).val,0), (yyvsp[0].dtype).val);
		   (yyval.dtype).stringval = 0;
		   (yyval.dtype).numval = 0;
		 }
 	       }
#line 11562 "CParse/parser.c"
    break;

  case 469: /* valexpr: LPAREN expr pointer AND RPAREN expr  */
#line 6898 "./CParse/parser.y"
                                                                          {
                 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).unary_arg_type = 0;
		 if ((yyvsp[0].dtype).type != T_STRING) {
		   SwigType_push((yyvsp[-4].dtype).val,(yyvsp[-3].type));
		   SwigType_add_reference((yyvsp[-4].dtype).val);
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[-4].dtype).val,0), (yyvsp[0].dtype).val);
		   (yyval.dtype).stringval = 0;
		   (yyval.dtype).numval = 0;
		 }
 	       }
#line 11578 "CParse/parser.c"
    break;

  case 470: /* valexpr: LPAREN expr pointer LAND RPAREN expr  */
#line 6909 "./CParse/parser.y"
                                                                           {
                 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).unary_arg_type = 0;
		 if ((yyvsp[0].dtype).type != T_STRING) {
		   SwigType_push((yyvsp[-4].dtype).val,(yyvsp[-3].type));
		   SwigType_add_rvalue_reference((yyvsp[-4].dtype).val);
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[-4].dtype).val,0), (yyvsp[0].dtype).val);
		   (yyval.dtype).stringval = 0;
		   (yyval.dtype).numval = 0;
		 }
 	       }
#line 11594 "CParse/parser.c"
    break;

  case 471: /* valexpr: AND expr  */
#line 6920 "./CParse/parser.y"
                          {
		 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).val = NewStringf("&%s", (yyvsp[0].dtype).val);
		 (yyval.dtype).stringval = 0;
		 (yyval.dtype).numval = 0;
		 /* Record the type code for expr so we can properly handle
		  * cases such as (6)&7 which get parsed using this rule then
		  * the rule for a C-style cast.
		  */
		 (yyval.dtype).unary_arg_type = (yyvsp[0].dtype).type;
		 switch ((yyval.dtype).type) {
		   case T_CHAR:
		     (yyval.dtype).type = T_STRING;
		     break;
		   case T_WCHAR:
		     (yyval.dtype).type = T_WSTRING;
		     break;
		   default:
		     (yyval.dtype).type = T_POINTER;
		 }
	       }
#line 11620 "CParse/parser.c"
    break;

  case 472: /* valexpr: STAR expr  */
#line 6941 "./CParse/parser.y"
                           {
		 (yyval.dtype) = (yyvsp[0].dtype);
		 (yyval.dtype).val = NewStringf("*%s", (yyvsp[0].dtype).val);
		 (yyval.dtype).stringval = 0;
		 (yyval.dtype).numval = 0;
		 /* Record the type code for expr so we can properly handle
		  * cases such as (6)*7 which get parsed using this rule then
		  * the rule for a C-style cast.
		  */
		 (yyval.dtype).unary_arg_type = (yyvsp[0].dtype).type;
		 switch ((yyval.dtype).type) {
		   case T_STRING:
		     (yyval.dtype).type = T_CHAR;
		     break;
		   case T_WSTRING:
		     (yyval.dtype).type = T_WCHAR;
		     break;
		   default:
		     (yyval.dtype).type = T_UNKNOWN;
		 }
	       }
#line 11646 "CParse/parser.c"
    break;

  case 483: /* exprcompound: expr PLUS expr  */
#line 6976 "./CParse/parser.y"
                                          {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s+%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11656 "CParse/parser.c"
    break;

  case 484: /* exprcompound: expr MINUS expr  */
#line 6981 "./CParse/parser.y"
                                           {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s-%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11666 "CParse/parser.c"
    break;

  case 485: /* exprcompound: expr STAR expr  */
#line 6986 "./CParse/parser.y"
                                          {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s*%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11676 "CParse/parser.c"
    break;

  case 486: /* exprcompound: expr SLASH expr  */
#line 6991 "./CParse/parser.y"
                                           {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s/%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11686 "CParse/parser.c"
    break;

  case 487: /* exprcompound: expr MODULO expr  */
#line 6996 "./CParse/parser.y"
                                            {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s%%%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11696 "CParse/parser.c"
    break;

  case 488: /* exprcompound: expr AND expr  */
#line 7001 "./CParse/parser.y"
                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s&%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11706 "CParse/parser.c"
    break;

  case 489: /* exprcompound: expr OR expr  */
#line 7006 "./CParse/parser.y"
                                        {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s|%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11716 "CParse/parser.c"
    break;

  case 490: /* exprcompound: expr XOR expr  */
#line 7011 "./CParse/parser.y"
                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s^%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type,(yyvsp[0].dtype).type);
	       }
#line 11726 "CParse/parser.c"
    break;

  case 491: /* exprcompound: expr LSHIFT expr  */
#line 7016 "./CParse/parser.y"
                                            {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s << %s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11736 "CParse/parser.c"
    break;

  case 492: /* exprcompound: expr RSHIFT expr  */
#line 7021 "./CParse/parser.y"
                                            {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s >> %s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11746 "CParse/parser.c"
    break;

  case 493: /* exprcompound: expr LAND expr  */
#line 7026 "./CParse/parser.y"
                                          {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s&&%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11756 "CParse/parser.c"
    break;

  case 494: /* exprcompound: expr LOR expr  */
#line 7031 "./CParse/parser.y"
                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s||%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11766 "CParse/parser.c"
    break;

  case 495: /* exprcompound: expr EQUALTO expr  */
#line 7036 "./CParse/parser.y"
                                             {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s==%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11776 "CParse/parser.c"
    break;

  case 496: /* exprcompound: expr NOTEQUALTO expr  */
#line 7041 "./CParse/parser.y"
                                                {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s!=%s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11786 "CParse/parser.c"
    break;

  case 497: /* exprcompound: LPAREN expr GREATERTHAN expr RPAREN  */
#line 7050 "./CParse/parser.y"
                                                               {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("(%s > %s)", (yyvsp[-3].dtype).val, (yyvsp[-1].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11796 "CParse/parser.c"
    break;

  case 498: /* exprcompound: LPAREN exprsimple LESSTHAN expr RPAREN  */
#line 7061 "./CParse/parser.y"
                                                                  {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("(%s < %s)", (yyvsp[-3].dtype).val, (yyvsp[-1].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11806 "CParse/parser.c"
    break;

  case 499: /* exprcompound: expr GREATERTHANOREQUALTO expr  */
#line 7066 "./CParse/parser.y"
                                                          {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s >= %s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11816 "CParse/parser.c"
    break;

  case 500: /* exprcompound: expr LESSTHANOREQUALTO expr  */
#line 7071 "./CParse/parser.y"
                                                       {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s <= %s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11826 "CParse/parser.c"
    break;

  case 501: /* exprcompound: expr PLUS ELLIPSIS  */
#line 7088 "./CParse/parser.y"
                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s+...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11836 "CParse/parser.c"
    break;

  case 502: /* exprcompound: expr MINUS ELLIPSIS  */
#line 7093 "./CParse/parser.y"
                                          {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s-...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11846 "CParse/parser.c"
    break;

  case 503: /* exprcompound: expr STAR ELLIPSIS  */
#line 7098 "./CParse/parser.y"
                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s*...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11856 "CParse/parser.c"
    break;

  case 504: /* exprcompound: expr SLASH ELLIPSIS  */
#line 7103 "./CParse/parser.y"
                                          {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s/...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11866 "CParse/parser.c"
    break;

  case 505: /* exprcompound: expr MODULO ELLIPSIS  */
#line 7108 "./CParse/parser.y"
                                           {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s%%...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11876 "CParse/parser.c"
    break;

  case 506: /* exprcompound: expr AND ELLIPSIS  */
#line 7113 "./CParse/parser.y"
                                        {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s&...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11886 "CParse/parser.c"
    break;

  case 507: /* exprcompound: expr OR ELLIPSIS  */
#line 7118 "./CParse/parser.y"
                                       {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s|...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11896 "CParse/parser.c"
    break;

  case 508: /* exprcompound: expr XOR ELLIPSIS  */
#line 7123 "./CParse/parser.y"
                                        {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s^...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11906 "CParse/parser.c"
    break;

  case 509: /* exprcompound: expr LSHIFT ELLIPSIS  */
#line 7128 "./CParse/parser.y"
                                           {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s << ...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11916 "CParse/parser.c"
    break;

  case 510: /* exprcompound: expr RSHIFT ELLIPSIS  */
#line 7133 "./CParse/parser.y"
                                           {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s >> ...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[-2].dtype).type);
	       }
#line 11926 "CParse/parser.c"
    break;

  case 511: /* exprcompound: expr LAND ELLIPSIS  */
#line 7138 "./CParse/parser.y"
                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s&&...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11936 "CParse/parser.c"
    break;

  case 512: /* exprcompound: expr LOR ELLIPSIS  */
#line 7143 "./CParse/parser.y"
                                        {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s||...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11946 "CParse/parser.c"
    break;

  case 513: /* exprcompound: expr EQUALTO ELLIPSIS  */
#line 7148 "./CParse/parser.y"
                                            {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s==...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11956 "CParse/parser.c"
    break;

  case 514: /* exprcompound: expr NOTEQUALTO ELLIPSIS  */
#line 7153 "./CParse/parser.y"
                                               {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s!=...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11966 "CParse/parser.c"
    break;

  case 515: /* exprcompound: LPAREN expr GREATERTHAN ELLIPSIS RPAREN  */
#line 7162 "./CParse/parser.y"
                                                              {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("(%s > ...)", (yyvsp[-3].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11976 "CParse/parser.c"
    break;

  case 516: /* exprcompound: LPAREN exprsimple LESSTHAN ELLIPSIS RPAREN  */
#line 7172 "./CParse/parser.y"
                                                                 {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("(%s < %s)", (yyvsp[-3].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11986 "CParse/parser.c"
    break;

  case 517: /* exprcompound: expr GREATERTHANOREQUALTO ELLIPSIS  */
#line 7177 "./CParse/parser.y"
                                                         {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s >= ...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 11996 "CParse/parser.c"
    break;

  case 518: /* exprcompound: expr LESSTHANOREQUALTO ELLIPSIS  */
#line 7182 "./CParse/parser.y"
                                                      {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s <= ...", (yyvsp[-2].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 12006 "CParse/parser.c"
    break;

  case 519: /* exprcompound: expr LESSEQUALGREATER expr  */
#line 7188 "./CParse/parser.y"
                                                      {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s <=> %s", (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 /* `<=>` returns one of `std::strong_ordering`,
		  * `std::partial_ordering` or `std::weak_ordering`.  The main
		  * thing to do with the return value in this context is to
		  * compare it with another ordering of the same type or
		  * with a literal 0.  We set .type = T_USER here which does
		  * what we want for the comparison operators, and also means
		  * that deduce_type() won't deduce a type for this (which is
		  * better than it deducing the wrong type).
		  */
		 (yyval.dtype).type = T_USER;
		 (yyval.dtype).unary_arg_type = 0;
	       }
#line 12026 "CParse/parser.c"
    break;

  case 520: /* exprcompound: expr QUESTIONMARK expr COLON expr  */
#line 7203 "./CParse/parser.y"
                                                                                           {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("%s?%s:%s", (yyvsp[-4].dtype).val, (yyvsp[-2].dtype).val, (yyvsp[0].dtype).val);
		 /* This may not be exactly right, but is probably good enough
		  * for the purposes of parsing constant expressions. */
		 (yyval.dtype).type = promote((yyvsp[-2].dtype).type, (yyvsp[0].dtype).type);
	       }
#line 12038 "CParse/parser.c"
    break;

  case 521: /* exprcompound: MINUS expr  */
#line 7210 "./CParse/parser.y"
                                             {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("-%s",(yyvsp[0].dtype).val);
		 if ((yyvsp[0].dtype).numval) {
		   switch ((yyvsp[0].dtype).type) {
		     case T_CHAR: // Unsigned on some architectures.
		     case T_UCHAR:
		     case T_USHORT:
		     case T_UINT:
		     case T_ULONG:
		     case T_ULONGLONG:
		       // Avoid negative numval with an unsigned type.
		       break;
		     default:
		       (yyval.dtype).numval = NewStringf("-%s", (yyvsp[0].dtype).numval);
		       break;
		   }
		   Delete((yyvsp[0].dtype).numval);
		 }
		 (yyval.dtype).type = promote_type((yyvsp[0].dtype).type);
		 /* Record the type code for expr so we can properly handle
		  * cases such as (6)-7 which get parsed using this rule then
		  * the rule for a C-style cast.
		  */
		 (yyval.dtype).unary_arg_type = (yyvsp[0].dtype).type;
	       }
#line 12069 "CParse/parser.c"
    break;

  case 522: /* exprcompound: PLUS expr  */
#line 7236 "./CParse/parser.y"
                                            {
		 (yyval.dtype) = default_dtype;
                 (yyval.dtype).val = NewStringf("+%s",(yyvsp[0].dtype).val);
		 (yyval.dtype).numval = (yyvsp[0].dtype).numval;
		 (yyval.dtype).type = promote_type((yyvsp[0].dtype).type);
		 /* Record the type code for expr so we can properly handle
		  * cases such as (6)+7 which get parsed using this rule then
		  * the rule for a C-style cast.
		  */
		 (yyval.dtype).unary_arg_type = (yyvsp[0].dtype).type;
	       }
#line 12085 "CParse/parser.c"
    break;

  case 523: /* exprcompound: NOT expr  */
#line 7247 "./CParse/parser.y"
                              {
		 (yyval.dtype) = default_dtype;
		 (yyval.dtype).val = NewStringf("~%s",(yyvsp[0].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[0].dtype).type);
	       }
#line 12095 "CParse/parser.c"
    break;

  case 524: /* exprcompound: LNOT expr  */
#line 7252 "./CParse/parser.y"
                               {
		 (yyval.dtype) = default_dtype;
                 (yyval.dtype).val = NewStringf("!%s", (yyvsp[0].dtype).val);
		 (yyval.dtype).type = cparse_cplusplus ? T_BOOL : T_INT;
	       }
#line 12105 "CParse/parser.c"
    break;

  case 525: /* exprcompound: type LPAREN  */
#line 7257 "./CParse/parser.y"
                             {
		 (yyval.dtype) = default_dtype;
		 String *qty;
		 if (skip_balanced('(',')') < 0) Exit(EXIT_FAILURE);
		 qty = Swig_symbol_type_qualify((yyvsp[-1].type),0);
		 if (SwigType_istemplate(qty)) {
		   String *nstr = SwigType_namestr(qty);
		   Delete(qty);
		   qty = nstr;
		 }
		 (yyval.dtype).val = NewStringf("%s%s",qty,scanner_ccode);
		 Clear(scanner_ccode);
		 /* Try to deduce the type - this could be a C++ "constructor
		  * cast" such as `double(4)` or a function call such as
		  * `some_func()`.  In the latter case we get T_USER, but that
		  * is wrong so we map it to T_UNKNOWN until we can actually
		  * deduce the return type of a function call (which is
		  * complicated because the return type can vary between
		  * overloaded forms).
		  */
		 (yyval.dtype).type = SwigType_type(qty);
		 if ((yyval.dtype).type == T_USER) (yyval.dtype).type = T_UNKNOWN;
		 (yyval.dtype).unary_arg_type = 0;
		 Delete(qty);
               }
#line 12135 "CParse/parser.c"
    break;

  case 526: /* variadic_opt: ELLIPSIS  */
#line 7284 "./CParse/parser.y"
                         {
		(yyval.intvalue) = 1;
	      }
#line 12143 "CParse/parser.c"
    break;

  case 527: /* variadic_opt: %empty  */
#line 7287 "./CParse/parser.y"
                       {
	        (yyval.intvalue) = 0;
	      }
#line 12151 "CParse/parser.c"
    break;

  case 529: /* $@12: %empty  */
#line 7295 "./CParse/parser.y"
                        { inherit_list = 1; }
#line 12157 "CParse/parser.c"
    break;

  case 530: /* raw_inherit: COLON $@12 base_list  */
#line 7295 "./CParse/parser.y"
                                                        { (yyval.bases) = (yyvsp[0].bases); inherit_list = 0; }
#line 12163 "CParse/parser.c"
    break;

  case 531: /* raw_inherit: %empty  */
#line 7296 "./CParse/parser.y"
                         { (yyval.bases) = 0; }
#line 12169 "CParse/parser.c"
    break;

  case 532: /* base_list: base_specifier  */
#line 7299 "./CParse/parser.y"
                                {
		   Hash *list = NewHash();
		   Node *base = (yyvsp[0].node);
		   Node *name = Getattr(base,"name");
		   List *lpublic = NewList();
		   List *lprotected = NewList();
		   List *lprivate = NewList();
		   Setattr(list,"public",lpublic);
		   Setattr(list,"protected",lprotected);
		   Setattr(list,"private",lprivate);
		   Delete(lpublic);
		   Delete(lprotected);
		   Delete(lprivate);
		   Append(Getattr(list,Getattr(base,"access")),name);
	           (yyval.bases) = list;
               }
#line 12190 "CParse/parser.c"
    break;

  case 533: /* base_list: base_list COMMA base_specifier  */
#line 7316 "./CParse/parser.y"
                                                    {
		   Hash *list = (yyvsp[-2].bases);
		   Node *base = (yyvsp[0].node);
		   Node *name = Getattr(base,"name");
		   Append(Getattr(list,Getattr(base,"access")),name);
                   (yyval.bases) = list;
               }
#line 12202 "CParse/parser.c"
    break;

  case 534: /* @13: %empty  */
#line 7325 "./CParse/parser.y"
                                       {
		 (yyval.intvalue) = cparse_line;
	       }
#line 12210 "CParse/parser.c"
    break;

  case 535: /* base_specifier: opt_virtual @13 idcolon variadic_opt  */
#line 7327 "./CParse/parser.y"
                                            {
		 (yyval.node) = NewHash();
		 Setfile((yyval.node), cparse_file);
		 Setline((yyval.node), (yyvsp[-2].intvalue));
		 Setattr((yyval.node), "name", (yyvsp[-1].str));
		 Setfile((yyvsp[-1].str), cparse_file);
		 Setline((yyvsp[-1].str), (yyvsp[-2].intvalue));
                 if (last_cpptype && (Strcmp(last_cpptype,"struct") != 0)) {
		   Setattr((yyval.node),"access","private");
		   Swig_warning(WARN_PARSE_NO_ACCESS, Getfile((yyval.node)), Getline((yyval.node)), "No access specifier given for base class '%s' (ignored).\n", SwigType_namestr((yyvsp[-1].str)));
                 } else {
		   Setattr((yyval.node),"access","public");
		 }
		 if ((yyvsp[0].intvalue)) {
		   SwigType_add_variadic(Getattr((yyval.node), "name"));
		 }
               }
#line 12232 "CParse/parser.c"
    break;

  case 536: /* @14: %empty  */
#line 7344 "./CParse/parser.y"
                                                        {
		 (yyval.intvalue) = cparse_line;
	       }
#line 12240 "CParse/parser.c"
    break;

  case 537: /* base_specifier: opt_virtual access_specifier @14 opt_virtual idcolon variadic_opt  */
#line 7346 "./CParse/parser.y"
                                                        {
		 (yyval.node) = NewHash();
		 Setfile((yyval.node), cparse_file);
		 Setline((yyval.node), (yyvsp[-3].intvalue));
		 Setattr((yyval.node), "name", (yyvsp[-1].str));
		 Setfile((yyvsp[-1].str), cparse_file);
		 Setline((yyvsp[-1].str), (yyvsp[-3].intvalue));
		 Setattr((yyval.node), "access", (yyvsp[-4].id));
		 if (Strcmp((yyvsp[-4].id), "public") != 0) {
		   Swig_warning(WARN_PARSE_PRIVATE_INHERIT, Getfile((yyval.node)), Getline((yyval.node)), "%s inheritance from base '%s' (ignored).\n", (yyvsp[-4].id), SwigType_namestr((yyvsp[-1].str)));
		 }
		 if ((yyvsp[0].intvalue)) {
		   SwigType_add_variadic(Getattr((yyval.node), "name"));
		 }
               }
#line 12260 "CParse/parser.c"
    break;

  case 538: /* access_specifier: PUBLIC  */
#line 7363 "./CParse/parser.y"
                           { (yyval.id) = "public"; }
#line 12266 "CParse/parser.c"
    break;

  case 539: /* access_specifier: PRIVATE  */
#line 7364 "./CParse/parser.y"
                         { (yyval.id) = "private"; }
#line 12272 "CParse/parser.c"
    break;

  case 540: /* access_specifier: PROTECTED  */
#line 7365 "./CParse/parser.y"
                           { (yyval.id) = "protected"; }
#line 12278 "CParse/parser.c"
    break;

  case 541: /* templcpptype: CLASS variadic_opt  */
#line 7368 "./CParse/parser.y"
                                    {
                   (yyval.type) = NewString("class");
		   if (!inherit_list) last_cpptype = (yyval.type);
		   if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.type));
               }
#line 12288 "CParse/parser.c"
    break;

  case 542: /* templcpptype: TYPENAME variadic_opt  */
#line 7373 "./CParse/parser.y"
                                       {
                   (yyval.type) = NewString("typename");
		   if (!inherit_list) last_cpptype = (yyval.type);
		   if ((yyvsp[0].intvalue)) SwigType_add_variadic((yyval.type));
               }
#line 12298 "CParse/parser.c"
    break;

  case 544: /* cpptype: STRUCT  */
#line 7381 "./CParse/parser.y"
                        {
                   (yyval.type) = NewString("struct");
		   if (!inherit_list) last_cpptype = (yyval.type);
               }
#line 12307 "CParse/parser.c"
    break;

  case 545: /* cpptype: UNION  */
#line 7385 "./CParse/parser.y"
                       {
                   (yyval.type) = NewString("union");
		   if (!inherit_list) last_cpptype = (yyval.type);
               }
#line 12316 "CParse/parser.c"
    break;

  case 546: /* classkey: CLASS  */
#line 7391 "./CParse/parser.y"
                       {
		   if (!inherit_list) last_cpptype = NewString("class");
               }
#line 12324 "CParse/parser.c"
    break;

  case 547: /* classkey: STRUCT  */
#line 7394 "./CParse/parser.y"
                        {
		   if (!inherit_list) last_cpptype = NewString("struct");
               }
#line 12332 "CParse/parser.c"
    break;

  case 548: /* classkey: UNION  */
#line 7397 "./CParse/parser.y"
                       {
		   if (!inherit_list) last_cpptype = NewString("union");
               }
#line 12340 "CParse/parser.c"
    break;

  case 553: /* virt_specifier_seq: OVERRIDE  */
#line 7410 "./CParse/parser.y"
                              {
                   (yyval.str) = 0;
	       }
#line 12348 "CParse/parser.c"
    break;

  case 554: /* virt_specifier_seq: FINAL  */
#line 7413 "./CParse/parser.y"
                       {
                   (yyval.str) = NewString("1");
	       }
#line 12356 "CParse/parser.c"
    break;

  case 555: /* virt_specifier_seq: FINAL OVERRIDE  */
#line 7416 "./CParse/parser.y"
                                {
                   (yyval.str) = NewString("1");
	       }
#line 12364 "CParse/parser.c"
    break;

  case 556: /* virt_specifier_seq: OVERRIDE FINAL  */
#line 7419 "./CParse/parser.y"
                                {
                   (yyval.str) = NewString("1");
	       }
#line 12372 "CParse/parser.c"
    break;

  case 558: /* virt_specifier_seq_opt: %empty  */
#line 7425 "./CParse/parser.y"
                        {
                   (yyval.str) = 0;
               }
#line 12380 "CParse/parser.c"
    break;

  case 559: /* class_virt_specifier_opt: FINAL  */
#line 7430 "./CParse/parser.y"
                                 {
                   (yyval.str) = NewString("1");
               }
#line 12388 "CParse/parser.c"
    break;

  case 560: /* class_virt_specifier_opt: %empty  */
#line 7433 "./CParse/parser.y"
                        {
                   (yyval.str) = 0;
               }
#line 12396 "CParse/parser.c"
    break;

  case 561: /* exception_specification: THROW LPAREN parms RPAREN  */
#line 7438 "./CParse/parser.y"
                                                    {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).throws = (yyvsp[-1].pl);
                    (yyval.dtype).throwf = NewString("1");
	       }
#line 12406 "CParse/parser.c"
    break;

  case 562: /* exception_specification: NOEXCEPT  */
#line 7443 "./CParse/parser.y"
                          {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).nexcept = NewString("true");
	       }
#line 12415 "CParse/parser.c"
    break;

  case 563: /* exception_specification: virt_specifier_seq  */
#line 7447 "./CParse/parser.y"
                                    {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).final = (yyvsp[0].str);
	       }
#line 12424 "CParse/parser.c"
    break;

  case 564: /* exception_specification: THROW LPAREN parms RPAREN virt_specifier_seq  */
#line 7451 "./CParse/parser.y"
                                                              {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).throws = (yyvsp[-2].pl);
                    (yyval.dtype).throwf = NewString("1");
                    (yyval.dtype).final = (yyvsp[0].str);
	       }
#line 12435 "CParse/parser.c"
    break;

  case 565: /* exception_specification: NOEXCEPT virt_specifier_seq  */
#line 7457 "./CParse/parser.y"
                                             {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).nexcept = NewString("true");
		    (yyval.dtype).final = (yyvsp[0].str);
	       }
#line 12445 "CParse/parser.c"
    break;

  case 566: /* exception_specification: NOEXCEPT LPAREN expr RPAREN  */
#line 7462 "./CParse/parser.y"
                                             {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).nexcept = (yyvsp[-1].dtype).val;
	       }
#line 12454 "CParse/parser.c"
    break;

  case 567: /* qualifiers_exception_specification: cv_ref_qualifier  */
#line 7468 "./CParse/parser.y"
                                                      {
		    (yyval.dtype) = default_dtype;
                    (yyval.dtype).qualifier = (yyvsp[0].dtype).qualifier;
                    (yyval.dtype).refqualifier = (yyvsp[0].dtype).refqualifier;
               }
#line 12464 "CParse/parser.c"
    break;

  case 568: /* qualifiers_exception_specification: exception_specification  */
#line 7473 "./CParse/parser.y"
                                         {
		    (yyval.dtype) = (yyvsp[0].dtype);
               }
#line 12472 "CParse/parser.c"
    break;

  case 569: /* qualifiers_exception_specification: cv_ref_qualifier exception_specification  */
#line 7476 "./CParse/parser.y"
                                                          {
		    (yyval.dtype) = (yyvsp[0].dtype);
                    (yyval.dtype).qualifier = (yyvsp[-1].dtype).qualifier;
                    (yyval.dtype).refqualifier = (yyvsp[-1].dtype).refqualifier;
               }
#line 12482 "CParse/parser.c"
    break;

  case 571: /* cpp_const: %empty  */
#line 7484 "./CParse/parser.y"
                        {
                 (yyval.dtype) = default_dtype;
               }
#line 12490 "CParse/parser.c"
    break;

  case 572: /* ctor_end: cpp_const ctor_initializer SEMI  */
#line 7489 "./CParse/parser.y"
                                                 { 
                    Clear(scanner_ccode); 
		    (yyval.decl) = default_decl;
		    (yyval.decl).throws = (yyvsp[-2].dtype).throws;
		    (yyval.decl).throwf = (yyvsp[-2].dtype).throwf;
		    (yyval.decl).nexcept = (yyvsp[-2].dtype).nexcept;
		    (yyval.decl).final = (yyvsp[-2].dtype).final;
                    if ((yyvsp[-2].dtype).qualifier)
                      Swig_error(cparse_file, cparse_line, "Constructor cannot have a qualifier.\n");
               }
#line 12505 "CParse/parser.c"
    break;

  case 573: /* ctor_end: cpp_const ctor_initializer LBRACE  */
#line 7499 "./CParse/parser.y"
                                                   { 
                    if ((yyvsp[-2].dtype).qualifier)
                      Swig_error(cparse_file, cparse_line, "Constructor cannot have a qualifier.\n");
                    if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		    (yyval.decl) = default_decl;
                    (yyval.decl).throws = (yyvsp[-2].dtype).throws;
                    (yyval.decl).throwf = (yyvsp[-2].dtype).throwf;
                    (yyval.decl).nexcept = (yyvsp[-2].dtype).nexcept;
                    (yyval.decl).final = (yyvsp[-2].dtype).final;
               }
#line 12520 "CParse/parser.c"
    break;

  case 574: /* ctor_end: LPAREN parms RPAREN SEMI  */
#line 7509 "./CParse/parser.y"
                                          { 
                    Clear(scanner_ccode); 
		    (yyval.decl) = default_decl;
                    (yyval.decl).parms = (yyvsp[-2].pl); 
                    (yyval.decl).have_parms = 1; 
               }
#line 12531 "CParse/parser.c"
    break;

  case 575: /* ctor_end: LPAREN parms RPAREN LBRACE  */
#line 7515 "./CParse/parser.y"
                                            {
                    if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		    (yyval.decl) = default_decl;
                    (yyval.decl).parms = (yyvsp[-2].pl); 
                    (yyval.decl).have_parms = 1; 
               }
#line 12542 "CParse/parser.c"
    break;

  case 576: /* ctor_end: EQUAL definetype SEMI  */
#line 7521 "./CParse/parser.y"
                                       { 
		    (yyval.decl) = default_decl;
                    (yyval.decl).defarg = (yyvsp[-1].dtype).val; 
		    (yyval.decl).stringdefarg = (yyvsp[-1].dtype).stringval;
		    (yyval.decl).numdefarg = (yyvsp[-1].dtype).numval;
               }
#line 12553 "CParse/parser.c"
    break;

  case 577: /* ctor_end: exception_specification EQUAL default_delete SEMI  */
#line 7527 "./CParse/parser.y"
                                                                   {
		    (yyval.decl) = default_decl;
                    (yyval.decl).defarg = (yyvsp[-1].dtype).val;
		    (yyval.decl).stringdefarg = (yyvsp[-1].dtype).stringval;
		    (yyval.decl).numdefarg = (yyvsp[-1].dtype).numval;
                    (yyval.decl).throws = (yyvsp[-3].dtype).throws;
                    (yyval.decl).throwf = (yyvsp[-3].dtype).throwf;
                    (yyval.decl).nexcept = (yyvsp[-3].dtype).nexcept;
                    (yyval.decl).final = (yyvsp[-3].dtype).final;
                    if ((yyvsp[-3].dtype).qualifier)
                      Swig_error(cparse_file, cparse_line, "Constructor cannot have a qualifier.\n");
               }
#line 12570 "CParse/parser.c"
    break;

  case 584: /* mem_initializer: idcolon LPAREN  */
#line 7551 "./CParse/parser.y"
                                 {
		  if (skip_balanced('(',')') < 0) Exit(EXIT_FAILURE);
		  Clear(scanner_ccode);
		}
#line 12579 "CParse/parser.c"
    break;

  case 585: /* mem_initializer: idcolon LBRACE  */
#line 7563 "./CParse/parser.y"
                                 {
		  if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		  Clear(scanner_ccode);
		}
#line 12588 "CParse/parser.c"
    break;

  case 586: /* less_valparms_greater: LESSTHAN valparms GREATERTHAN  */
#line 7569 "./CParse/parser.y"
                                                      {
                     String *s = NewStringEmpty();
                     SwigType_add_template(s,(yyvsp[-1].p));
		     (yyval.str) = s;
		     scanner_last_id(1);
                }
#line 12599 "CParse/parser.c"
    break;

  case 588: /* identifier: OVERRIDE  */
#line 7579 "./CParse/parser.y"
                          { (yyval.id) = Swig_copy_string("override"); }
#line 12605 "CParse/parser.c"
    break;

  case 589: /* identifier: FINAL  */
#line 7580 "./CParse/parser.y"
                       { (yyval.id) = Swig_copy_string("final"); }
#line 12611 "CParse/parser.c"
    break;

  case 591: /* idstring: default_delete  */
#line 7584 "./CParse/parser.y"
                                { (yyval.id) = Char((yyvsp[0].dtype).val); }
#line 12617 "CParse/parser.c"
    break;

  case 592: /* idstring: string  */
#line 7585 "./CParse/parser.y"
                        { (yyval.id) = Char((yyvsp[0].str)); }
#line 12623 "CParse/parser.c"
    break;

  case 594: /* idstringopt: %empty  */
#line 7589 "./CParse/parser.y"
                        { (yyval.id) = 0; }
#line 12629 "CParse/parser.c"
    break;

  case 595: /* idcolon: idtemplate idcolontail  */
#line 7592 "./CParse/parser.y"
                                        { 
		 (yyval.str) = NewStringf("%s%s", (yyvsp[-1].str), (yyvsp[0].str));
		 Delete((yyvsp[0].str));
               }
#line 12638 "CParse/parser.c"
    break;

  case 596: /* idcolon: NONID DCOLON idtemplatetemplate idcolontail  */
#line 7596 "./CParse/parser.y"
                                                             {
		 (yyval.str) = NewStringf("::%s%s",(yyvsp[-1].str),(yyvsp[0].str));
                 Delete((yyvsp[0].str));
               }
#line 12647 "CParse/parser.c"
    break;

  case 597: /* idcolon: idtemplate  */
#line 7600 "./CParse/parser.y"
                            {
		 (yyval.str) = NewString((yyvsp[0].str));
   	       }
#line 12655 "CParse/parser.c"
    break;

  case 598: /* idcolon: NONID DCOLON idtemplatetemplate  */
#line 7603 "./CParse/parser.y"
                                                 {
		 (yyval.str) = NewStringf("::%s",(yyvsp[0].str));
               }
#line 12663 "CParse/parser.c"
    break;

  case 599: /* idcolon: OPERATOR  */
#line 7606 "./CParse/parser.y"
                                    {
		 (yyval.str) = (yyvsp[0].str);
	       }
#line 12671 "CParse/parser.c"
    break;

  case 600: /* idcolon: OPERATOR less_valparms_greater  */
#line 7609 "./CParse/parser.y"
                                                {
		 (yyval.str) = (yyvsp[-1].str);
		 Append((yyval.str), (yyvsp[0].str));
		 Delete((yyvsp[0].str));
	       }
#line 12681 "CParse/parser.c"
    break;

  case 601: /* idcolon: NONID DCOLON OPERATOR  */
#line 7614 "./CParse/parser.y"
                                       {
		 (yyval.str) = (yyvsp[0].str);
		 Insert((yyval.str), 0, "::");
               }
#line 12690 "CParse/parser.c"
    break;

  case 602: /* idcolontail: DCOLON idtemplatetemplate idcolontail  */
#line 7620 "./CParse/parser.y"
                                                           {
                   (yyval.str) = NewStringf("::%s%s",(yyvsp[-1].str),(yyvsp[0].str));
		   Delete((yyvsp[0].str));
               }
#line 12699 "CParse/parser.c"
    break;

  case 603: /* idcolontail: DCOLON idtemplatetemplate  */
#line 7624 "./CParse/parser.y"
                                           {
                   (yyval.str) = NewStringf("::%s",(yyvsp[0].str));
               }
#line 12707 "CParse/parser.c"
    break;

  case 604: /* idcolontail: DCOLON OPERATOR  */
#line 7627 "./CParse/parser.y"
                                 {
		   (yyval.str) = (yyvsp[0].str);
		   Insert((yyval.str), 0, "::");
               }
#line 12716 "CParse/parser.c"
    break;

  case 605: /* idcolontail: DCNOT idtemplate  */
#line 7631 "./CParse/parser.y"
                                  {
		 (yyval.str) = NewStringf("::~%s",(yyvsp[0].str));
               }
#line 12724 "CParse/parser.c"
    break;

  case 606: /* idtemplate: identifier  */
#line 7637 "./CParse/parser.y"
                           {
		(yyval.str) = NewString((yyvsp[0].id));
	      }
#line 12732 "CParse/parser.c"
    break;

  case 607: /* idtemplate: identifier less_valparms_greater  */
#line 7640 "./CParse/parser.y"
                                                 {
		(yyval.str) = NewString((yyvsp[-1].id));
		Append((yyval.str), (yyvsp[0].str));
		Delete((yyvsp[0].str));
	      }
#line 12742 "CParse/parser.c"
    break;

  case 609: /* idtemplatetemplate: TEMPLATE identifier less_valparms_greater  */
#line 7648 "./CParse/parser.y"
                                                          {
		(yyval.str) = NewString((yyvsp[-1].id));
		Append((yyval.str), (yyvsp[0].str));
		Delete((yyvsp[0].str));
	      }
#line 12752 "CParse/parser.c"
    break;

  case 610: /* idcolonnt: identifier idcolontailnt  */
#line 7656 "./CParse/parser.y"
                                         {
		 (yyval.str) = NewStringf("%s%s", (yyvsp[-1].id), (yyvsp[0].str));
		 Delete((yyvsp[0].str));
               }
#line 12761 "CParse/parser.c"
    break;

  case 611: /* idcolonnt: NONID DCOLON identifier idcolontailnt  */
#line 7660 "./CParse/parser.y"
                                                       {
		 (yyval.str) = NewStringf("::%s%s",(yyvsp[-1].id),(yyvsp[0].str));
                 Delete((yyvsp[0].str));
               }
#line 12770 "CParse/parser.c"
    break;

  case 612: /* idcolonnt: identifier  */
#line 7664 "./CParse/parser.y"
                            {
		 (yyval.str) = NewString((yyvsp[0].id));
   	       }
#line 12778 "CParse/parser.c"
    break;

  case 613: /* idcolonnt: NONID DCOLON identifier  */
#line 7667 "./CParse/parser.y"
                                         {
		 (yyval.str) = NewStringf("::%s",(yyvsp[0].id));
               }
#line 12786 "CParse/parser.c"
    break;

  case 614: /* idcolonnt: OPERATOR  */
#line 7670 "./CParse/parser.y"
                          {
		 (yyval.str) = (yyvsp[0].str);
	       }
#line 12794 "CParse/parser.c"
    break;

  case 615: /* idcolonnt: NONID DCOLON OPERATOR  */
#line 7673 "./CParse/parser.y"
                                       {
		 (yyval.str) = (yyvsp[0].str);
		 Insert((yyval.str), 0, "::");
               }
#line 12803 "CParse/parser.c"
    break;

  case 616: /* idcolontailnt: DCOLON identifier idcolontailnt  */
#line 7679 "./CParse/parser.y"
                                                      {
                   (yyval.str) = NewStringf("::%s%s",(yyvsp[-1].id),(yyvsp[0].str));
		   Delete((yyvsp[0].str));
               }
#line 12812 "CParse/parser.c"
    break;

  case 617: /* idcolontailnt: DCOLON identifier  */
#line 7683 "./CParse/parser.y"
                                   {
                   (yyval.str) = NewStringf("::%s",(yyvsp[0].id));
               }
#line 12820 "CParse/parser.c"
    break;

  case 618: /* idcolontailnt: DCOLON OPERATOR  */
#line 7686 "./CParse/parser.y"
                                 {
		   (yyval.str) = (yyvsp[0].str);
		   Insert((yyval.str), 0, "::");
               }
#line 12829 "CParse/parser.c"
    break;

  case 619: /* idcolontailnt: DCNOT identifier  */
#line 7690 "./CParse/parser.y"
                                  {
		 (yyval.str) = NewStringf("::~%s",(yyvsp[0].id));
               }
#line 12837 "CParse/parser.c"
    break;

  case 620: /* string: string STRING  */
#line 7696 "./CParse/parser.y"
                                   { 
		   (yyval.str) = (yyvsp[-1].str);
		   Append((yyval.str), (yyvsp[0].str));
		   Delete((yyvsp[0].str));
	       }
#line 12847 "CParse/parser.c"
    break;

  case 622: /* wstring: wstring WSTRING  */
#line 7703 "./CParse/parser.y"
                                     {
		   // Concatenated wide strings: L"str1" L"str2"
		   (yyval.str) = (yyvsp[-1].str);
		   Append((yyval.str), (yyvsp[0].str));
		   Delete((yyvsp[0].str));
	       }
#line 12858 "CParse/parser.c"
    break;

  case 623: /* wstring: wstring STRING  */
#line 7709 "./CParse/parser.y"
                                    {
		   // Concatenated wide string and normal string literal: L"str1" "str2" (C++11).
		   (yyval.str) = (yyvsp[-1].str);
		   Append((yyval.str), (yyvsp[0].str));
		   Delete((yyvsp[0].str));
	       }
#line 12869 "CParse/parser.c"
    break;

  case 624: /* wstring: string WSTRING  */
#line 7715 "./CParse/parser.y"
                                    {
		   // Concatenated normal string and wide string literal: "str1" L"str2" (C++11).
		   (yyval.str) = (yyvsp[-1].str);
		   Append((yyval.str), (yyvsp[0].str));
		   Delete((yyvsp[0].str));
	       }
#line 12880 "CParse/parser.c"
    break;

  case 627: /* stringbrace: LBRACE  */
#line 7725 "./CParse/parser.y"
                        {
		  if (skip_balanced('{','}') < 0) Exit(EXIT_FAILURE);
		  (yyval.str) = NewString(scanner_ccode);
               }
#line 12889 "CParse/parser.c"
    break;

  case 629: /* options: LPAREN kwargs RPAREN  */
#line 7732 "./CParse/parser.y"
                                      {
                  Hash *n;
                  (yyval.node) = NewHash();
                  n = (yyvsp[-1].node);
                  while(n) {
                     String *name, *value;
                     name = Getattr(n,"name");
                     value = Getattr(n,"value");
		     if (!value) value = (String *) "1";
                     Setattr((yyval.node),name, value);
		     n = nextSibling(n);
		  }
               }
#line 12907 "CParse/parser.c"
    break;

  case 630: /* options: %empty  */
#line 7745 "./CParse/parser.y"
                        { (yyval.node) = 0; }
#line 12913 "CParse/parser.c"
    break;

  case 631: /* kwargs: kwargs_builder  */
#line 7749 "./CParse/parser.y"
                                {
		 (yyval.node) = (yyvsp[0].nodebuilder).node;
	       }
#line 12921 "CParse/parser.c"
    break;

  case 632: /* kwargs_builder: idstring EQUAL stringnum  */
#line 7754 "./CParse/parser.y"
                                          {
		 Node *n = NewHash();
		 Setattr(n, "name", (yyvsp[-2].id));
		 Setattr(n, "value", (yyvsp[0].str));
		 Delete((yyvsp[0].str));
		 (yyval.nodebuilder).node = (yyval.nodebuilder).last = n;
	       }
#line 12933 "CParse/parser.c"
    break;

  case 633: /* kwargs_builder: kwargs_builder COMMA idstring EQUAL stringnum  */
#line 7761 "./CParse/parser.y"
                                                                   {
		 (yyval.nodebuilder) = (yyvsp[-4].nodebuilder);
		 Node *n = NewHash();
		 Setattr(n, "name", (yyvsp[-2].id));
		 Setattr(n, "value", (yyvsp[0].str));
		 Delete((yyvsp[0].str));
		 set_nextSibling((yyval.nodebuilder).last, n);
		 (yyval.nodebuilder).last = n;
	       }
#line 12947 "CParse/parser.c"
    break;

  case 634: /* kwargs_builder: idstring  */
#line 7770 "./CParse/parser.y"
                          {
		 Node *n = NewHash();
		 Setattr(n, "name", (yyvsp[0].id));
		 (yyval.nodebuilder).node = (yyval.nodebuilder).last = n;
	       }
#line 12957 "CParse/parser.c"
    break;

  case 635: /* kwargs_builder: kwargs_builder COMMA idstring  */
#line 7775 "./CParse/parser.y"
                                                   {
		 (yyval.nodebuilder) = (yyvsp[-2].nodebuilder);
		 Node *n = NewHash();
		 Setattr(n, "name", (yyvsp[0].id));
		 set_nextSibling((yyval.nodebuilder).last, n);
		 (yyval.nodebuilder).last = n;
	       }
#line 12969 "CParse/parser.c"
    break;

  case 636: /* kwargs_builder: idstring EQUAL stringtype  */
#line 7782 "./CParse/parser.y"
                                           {
		 Node *n = (yyvsp[0].node);
		 Setattr(n, "name", (yyvsp[-2].id));
		 (yyval.nodebuilder).node = (yyval.nodebuilder).last = n;
	       }
#line 12979 "CParse/parser.c"
    break;

  case 637: /* kwargs_builder: kwargs_builder COMMA idstring EQUAL stringtype  */
#line 7787 "./CParse/parser.y"
                                                                    {
		 (yyval.nodebuilder) = (yyvsp[-4].nodebuilder);
		 Node *n = (yyvsp[0].node);
		 Setattr(n, "name", (yyvsp[-2].id));
		 set_nextSibling((yyval.nodebuilder).last, n);
		 (yyval.nodebuilder).last = n;
	       }
#line 12991 "CParse/parser.c"
    break;

  case 639: /* stringnum: exprnum  */
#line 7797 "./CParse/parser.y"
                         {
		 (yyval.str) = (yyvsp[0].dtype).val;
               }
#line 12999 "CParse/parser.c"
    break;


#line 13003 "CParse/parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= END)
        {
          /* Return failure if at end of input.  */
          if (yychar == END)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 7802 "./CParse/parser.y"


SwigType *Swig_cparse_type(String *s) {
   String *ns;
   ns = NewString(s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSETYPE);
   if (yyparse() == 2) {
      Swig_error(cparse_file, cparse_line, "Parser exceeded stack depth or ran out of memory\n");
      Exit(EXIT_FAILURE);
   }
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   return (SwigType *)top;
}


Parm *Swig_cparse_parm(String *s) {
   String *ns;
   ns = NewString(s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSEPARM);
   if (yyparse() == 2) {
      Swig_error(cparse_file, cparse_line, "Parser exceeded stack depth or ran out of memory\n");
      Exit(EXIT_FAILURE);
   }
   /*   Printf(stdout,"parmparse: '%s' ---> '%s'\n", s, top); */
   Delete(ns);
   return (Parm *)top;
}


ParmList *Swig_cparse_parms(String *s, Node *file_line_node) {
   String *ns;
   char *cs = Char(s);
   if (cs && cs[0] != '(') {
     ns = NewStringf("(%s)",s);
   } else {
     ns = NewString(s);
   }
   Setfile(ns, Getfile(file_line_node));
   Setline(ns, Getline(file_line_node));
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSEPARMS);
   if (yyparse() == 2) {
      Swig_error(cparse_file, cparse_line, "Parser exceeded stack depth or ran out of memory\n");
      Exit(EXIT_FAILURE);
   }
   /*   Printf(stdout,"parmsparse: '%s' ---> '%s'\n", s, top); */
   return (ParmList *)top;
}

