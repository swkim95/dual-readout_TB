// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__drcTB
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/Users/swkim/DRC/dual-readout_TB/include/TBdetector.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBevt.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBmid.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBmonit.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBplot.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBread.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TButility.h"
#include "TBmid.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *TBwaveform_Dictionary();
   static void TBwaveform_TClassManip(TClass*);
   static void *new_TBwaveform(void *p = nullptr);
   static void *newArray_TBwaveform(Long_t size, void *p);
   static void delete_TBwaveform(void *p);
   static void deleteArray_TBwaveform(void *p);
   static void destruct_TBwaveform(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBwaveform*)
   {
      ::TBwaveform *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBwaveform));
      static ::ROOT::TGenericClassInfo 
         instance("TBwaveform", "TBmid.h", 8,
                  typeid(::TBwaveform), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBwaveform_Dictionary, isa_proxy, 4,
                  sizeof(::TBwaveform) );
      instance.SetNew(&new_TBwaveform);
      instance.SetNewArray(&newArray_TBwaveform);
      instance.SetDelete(&delete_TBwaveform);
      instance.SetDeleteArray(&deleteArray_TBwaveform);
      instance.SetDestructor(&destruct_TBwaveform);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBwaveform*)
   {
      return GenerateInitInstanceLocal((::TBwaveform*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBwaveform*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBwaveform_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBwaveform*)nullptr)->GetClass();
      TBwaveform_TClassManip(theClass);
   return theClass;
   }

   static void TBwaveform_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBfastmode_Dictionary();
   static void TBfastmode_TClassManip(TClass*);
   static void *new_TBfastmode(void *p = nullptr);
   static void *newArray_TBfastmode(Long_t size, void *p);
   static void delete_TBfastmode(void *p);
   static void deleteArray_TBfastmode(void *p);
   static void destruct_TBfastmode(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBfastmode*)
   {
      ::TBfastmode *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBfastmode));
      static ::ROOT::TGenericClassInfo 
         instance("TBfastmode", "TBmid.h", 30,
                  typeid(::TBfastmode), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBfastmode_Dictionary, isa_proxy, 4,
                  sizeof(::TBfastmode) );
      instance.SetNew(&new_TBfastmode);
      instance.SetNewArray(&newArray_TBfastmode);
      instance.SetDelete(&delete_TBfastmode);
      instance.SetDeleteArray(&deleteArray_TBfastmode);
      instance.SetDestructor(&destruct_TBfastmode);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBfastmode*)
   {
      return GenerateInitInstanceLocal((::TBfastmode*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBfastmode*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBfastmode_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBfastmode*)nullptr)->GetClass();
      TBfastmode_TClassManip(theClass);
   return theClass;
   }

   static void TBfastmode_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBmidbase_Dictionary();
   static void TBmidbase_TClassManip(TClass*);
   static void *new_TBmidbase(void *p = nullptr);
   static void *newArray_TBmidbase(Long_t size, void *p);
   static void delete_TBmidbase(void *p);
   static void deleteArray_TBmidbase(void *p);
   static void destruct_TBmidbase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBmidbase*)
   {
      ::TBmidbase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBmidbase));
      static ::ROOT::TGenericClassInfo 
         instance("TBmidbase", "TBmid.h", 49,
                  typeid(::TBmidbase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBmidbase_Dictionary, isa_proxy, 4,
                  sizeof(::TBmidbase) );
      instance.SetNew(&new_TBmidbase);
      instance.SetNewArray(&newArray_TBmidbase);
      instance.SetDelete(&delete_TBmidbase);
      instance.SetDeleteArray(&deleteArray_TBmidbase);
      instance.SetDestructor(&destruct_TBmidbase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBmidbase*)
   {
      return GenerateInitInstanceLocal((::TBmidbase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBmidbase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBmidbase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBmidbase*)nullptr)->GetClass();
      TBmidbase_TClassManip(theClass);
   return theClass;
   }

   static void TBmidbase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBmidlETBwaveformgR_Dictionary();
   static void TBmidlETBwaveformgR_TClassManip(TClass*);
   static void *new_TBmidlETBwaveformgR(void *p = nullptr);
   static void *newArray_TBmidlETBwaveformgR(Long_t size, void *p);
   static void delete_TBmidlETBwaveformgR(void *p);
   static void deleteArray_TBmidlETBwaveformgR(void *p);
   static void destruct_TBmidlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBmid<TBwaveform>*)
   {
      ::TBmid<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBmid<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("TBmid<TBwaveform>", "TBmid.h", 88,
                  typeid(::TBmid<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBmidlETBwaveformgR_Dictionary, isa_proxy, 4,
                  sizeof(::TBmid<TBwaveform>) );
      instance.SetNew(&new_TBmidlETBwaveformgR);
      instance.SetNewArray(&newArray_TBmidlETBwaveformgR);
      instance.SetDelete(&delete_TBmidlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_TBmidlETBwaveformgR);
      instance.SetDestructor(&destruct_TBmidlETBwaveformgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBmid<TBwaveform>*)
   {
      return GenerateInitInstanceLocal((::TBmid<TBwaveform>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBmid<TBwaveform>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBmidlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBmid<TBwaveform>*)nullptr)->GetClass();
      TBmidlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void TBmidlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBmidlETBfastmodegR_Dictionary();
   static void TBmidlETBfastmodegR_TClassManip(TClass*);
   static void *new_TBmidlETBfastmodegR(void *p = nullptr);
   static void *newArray_TBmidlETBfastmodegR(Long_t size, void *p);
   static void delete_TBmidlETBfastmodegR(void *p);
   static void deleteArray_TBmidlETBfastmodegR(void *p);
   static void destruct_TBmidlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBmid<TBfastmode>*)
   {
      ::TBmid<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBmid<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("TBmid<TBfastmode>", "TBmid.h", 88,
                  typeid(::TBmid<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBmidlETBfastmodegR_Dictionary, isa_proxy, 4,
                  sizeof(::TBmid<TBfastmode>) );
      instance.SetNew(&new_TBmidlETBfastmodegR);
      instance.SetNewArray(&newArray_TBmidlETBfastmodegR);
      instance.SetDelete(&delete_TBmidlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_TBmidlETBfastmodegR);
      instance.SetDestructor(&destruct_TBmidlETBfastmodegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBmid<TBfastmode>*)
   {
      return GenerateInitInstanceLocal((::TBmid<TBfastmode>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBmid<TBfastmode>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBmidlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBmid<TBfastmode>*)nullptr)->GetClass();
      TBmidlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void TBmidlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBcid_Dictionary();
   static void TBcid_TClassManip(TClass*);
   static void delete_TBcid(void *p);
   static void deleteArray_TBcid(void *p);
   static void destruct_TBcid(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBcid*)
   {
      ::TBcid *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBcid));
      static ::ROOT::TGenericClassInfo 
         instance("TBcid", "TBdetector.h", 7,
                  typeid(::TBcid), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBcid_Dictionary, isa_proxy, 4,
                  sizeof(::TBcid) );
      instance.SetDelete(&delete_TBcid);
      instance.SetDeleteArray(&deleteArray_TBcid);
      instance.SetDestructor(&destruct_TBcid);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBcid*)
   {
      return GenerateInitInstanceLocal((::TBcid*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBcid*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBcid_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBcid*)nullptr)->GetClass();
      TBcid_TClassManip(theClass);
   return theClass;
   }

   static void TBcid_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBevtlETBfastmodegR_Dictionary();
   static void TBevtlETBfastmodegR_TClassManip(TClass*);
   static void *new_TBevtlETBfastmodegR(void *p = nullptr);
   static void *newArray_TBevtlETBfastmodegR(Long_t size, void *p);
   static void delete_TBevtlETBfastmodegR(void *p);
   static void deleteArray_TBevtlETBfastmodegR(void *p);
   static void destruct_TBevtlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBevt<TBfastmode>*)
   {
      ::TBevt<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBevt<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("TBevt<TBfastmode>", "TBevt.h", 10,
                  typeid(::TBevt<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBevtlETBfastmodegR_Dictionary, isa_proxy, 4,
                  sizeof(::TBevt<TBfastmode>) );
      instance.SetNew(&new_TBevtlETBfastmodegR);
      instance.SetNewArray(&newArray_TBevtlETBfastmodegR);
      instance.SetDelete(&delete_TBevtlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_TBevtlETBfastmodegR);
      instance.SetDestructor(&destruct_TBevtlETBfastmodegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBevt<TBfastmode>*)
   {
      return GenerateInitInstanceLocal((::TBevt<TBfastmode>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBevt<TBfastmode>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBevtlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBevt<TBfastmode>*)nullptr)->GetClass();
      TBevtlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void TBevtlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBevtlETBwaveformgR_Dictionary();
   static void TBevtlETBwaveformgR_TClassManip(TClass*);
   static void *new_TBevtlETBwaveformgR(void *p = nullptr);
   static void *newArray_TBevtlETBwaveformgR(Long_t size, void *p);
   static void delete_TBevtlETBwaveformgR(void *p);
   static void deleteArray_TBevtlETBwaveformgR(void *p);
   static void destruct_TBevtlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBevt<TBwaveform>*)
   {
      ::TBevt<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBevt<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("TBevt<TBwaveform>", "TBevt.h", 10,
                  typeid(::TBevt<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBevtlETBwaveformgR_Dictionary, isa_proxy, 4,
                  sizeof(::TBevt<TBwaveform>) );
      instance.SetNew(&new_TBevtlETBwaveformgR);
      instance.SetNewArray(&newArray_TBevtlETBwaveformgR);
      instance.SetDelete(&delete_TBevtlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_TBevtlETBwaveformgR);
      instance.SetDestructor(&destruct_TBevtlETBwaveformgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBevt<TBwaveform>*)
   {
      return GenerateInitInstanceLocal((::TBevt<TBwaveform>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TBevt<TBwaveform>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBevtlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TBevt<TBwaveform>*)nullptr)->GetClass();
      TBevtlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void TBevtlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBwaveform(void *p) {
      return  p ? new(p) ::TBwaveform : new ::TBwaveform;
   }
   static void *newArray_TBwaveform(Long_t nElements, void *p) {
      return p ? new(p) ::TBwaveform[nElements] : new ::TBwaveform[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBwaveform(void *p) {
      delete ((::TBwaveform*)p);
   }
   static void deleteArray_TBwaveform(void *p) {
      delete [] ((::TBwaveform*)p);
   }
   static void destruct_TBwaveform(void *p) {
      typedef ::TBwaveform current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBwaveform

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBfastmode(void *p) {
      return  p ? new(p) ::TBfastmode : new ::TBfastmode;
   }
   static void *newArray_TBfastmode(Long_t nElements, void *p) {
      return p ? new(p) ::TBfastmode[nElements] : new ::TBfastmode[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBfastmode(void *p) {
      delete ((::TBfastmode*)p);
   }
   static void deleteArray_TBfastmode(void *p) {
      delete [] ((::TBfastmode*)p);
   }
   static void destruct_TBfastmode(void *p) {
      typedef ::TBfastmode current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBfastmode

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBmidbase(void *p) {
      return  p ? new(p) ::TBmidbase : new ::TBmidbase;
   }
   static void *newArray_TBmidbase(Long_t nElements, void *p) {
      return p ? new(p) ::TBmidbase[nElements] : new ::TBmidbase[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBmidbase(void *p) {
      delete ((::TBmidbase*)p);
   }
   static void deleteArray_TBmidbase(void *p) {
      delete [] ((::TBmidbase*)p);
   }
   static void destruct_TBmidbase(void *p) {
      typedef ::TBmidbase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBmidbase

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBmidlETBwaveformgR(void *p) {
      return  p ? new(p) ::TBmid<TBwaveform> : new ::TBmid<TBwaveform>;
   }
   static void *newArray_TBmidlETBwaveformgR(Long_t nElements, void *p) {
      return p ? new(p) ::TBmid<TBwaveform>[nElements] : new ::TBmid<TBwaveform>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBmidlETBwaveformgR(void *p) {
      delete ((::TBmid<TBwaveform>*)p);
   }
   static void deleteArray_TBmidlETBwaveformgR(void *p) {
      delete [] ((::TBmid<TBwaveform>*)p);
   }
   static void destruct_TBmidlETBwaveformgR(void *p) {
      typedef ::TBmid<TBwaveform> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBmid<TBwaveform>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBmidlETBfastmodegR(void *p) {
      return  p ? new(p) ::TBmid<TBfastmode> : new ::TBmid<TBfastmode>;
   }
   static void *newArray_TBmidlETBfastmodegR(Long_t nElements, void *p) {
      return p ? new(p) ::TBmid<TBfastmode>[nElements] : new ::TBmid<TBfastmode>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBmidlETBfastmodegR(void *p) {
      delete ((::TBmid<TBfastmode>*)p);
   }
   static void deleteArray_TBmidlETBfastmodegR(void *p) {
      delete [] ((::TBmid<TBfastmode>*)p);
   }
   static void destruct_TBmidlETBfastmodegR(void *p) {
      typedef ::TBmid<TBfastmode> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBmid<TBfastmode>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TBcid(void *p) {
      delete ((::TBcid*)p);
   }
   static void deleteArray_TBcid(void *p) {
      delete [] ((::TBcid*)p);
   }
   static void destruct_TBcid(void *p) {
      typedef ::TBcid current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBcid

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBevtlETBfastmodegR(void *p) {
      return  p ? new(p) ::TBevt<TBfastmode> : new ::TBevt<TBfastmode>;
   }
   static void *newArray_TBevtlETBfastmodegR(Long_t nElements, void *p) {
      return p ? new(p) ::TBevt<TBfastmode>[nElements] : new ::TBevt<TBfastmode>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBevtlETBfastmodegR(void *p) {
      delete ((::TBevt<TBfastmode>*)p);
   }
   static void deleteArray_TBevtlETBfastmodegR(void *p) {
      delete [] ((::TBevt<TBfastmode>*)p);
   }
   static void destruct_TBevtlETBfastmodegR(void *p) {
      typedef ::TBevt<TBfastmode> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBevt<TBfastmode>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBevtlETBwaveformgR(void *p) {
      return  p ? new(p) ::TBevt<TBwaveform> : new ::TBevt<TBwaveform>;
   }
   static void *newArray_TBevtlETBwaveformgR(Long_t nElements, void *p) {
      return p ? new(p) ::TBevt<TBwaveform>[nElements] : new ::TBevt<TBwaveform>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBevtlETBwaveformgR(void *p) {
      delete ((::TBevt<TBwaveform>*)p);
   }
   static void deleteArray_TBevtlETBwaveformgR(void *p) {
      delete [] ((::TBevt<TBwaveform>*)p);
   }
   static void destruct_TBevtlETBwaveformgR(void *p) {
      typedef ::TBevt<TBwaveform> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBevt<TBwaveform>

namespace ROOT {
   static TClass *vectorlEshortgR_Dictionary();
   static void vectorlEshortgR_TClassManip(TClass*);
   static void *new_vectorlEshortgR(void *p = nullptr);
   static void *newArray_vectorlEshortgR(Long_t size, void *p);
   static void delete_vectorlEshortgR(void *p);
   static void deleteArray_vectorlEshortgR(void *p);
   static void destruct_vectorlEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<short>*)
   {
      vector<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<short>", -2, "c++/v1/vector", 478,
                  typeid(vector<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<short>) );
      instance.SetNew(&new_vectorlEshortgR);
      instance.SetNewArray(&newArray_vectorlEshortgR);
      instance.SetDelete(&delete_vectorlEshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEshortgR);
      instance.SetDestructor(&destruct_vectorlEshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<short> >()));

      ::ROOT::AddClassAlternate("vector<short>","std::__1::vector<short, std::__1::allocator<short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<short>*)nullptr)->GetClass();
      vectorlEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short> : new vector<short>;
   }
   static void *newArray_vectorlEshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short>[nElements] : new vector<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEshortgR(void *p) {
      delete ((vector<short>*)p);
   }
   static void deleteArray_vectorlEshortgR(void *p) {
      delete [] ((vector<short>*)p);
   }
   static void destruct_vectorlEshortgR(void *p) {
      typedef vector<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<short>

namespace ROOT {
   static TClass *vectorlETBwaveformgR_Dictionary();
   static void vectorlETBwaveformgR_TClassManip(TClass*);
   static void *new_vectorlETBwaveformgR(void *p = nullptr);
   static void *newArray_vectorlETBwaveformgR(Long_t size, void *p);
   static void delete_vectorlETBwaveformgR(void *p);
   static void deleteArray_vectorlETBwaveformgR(void *p);
   static void destruct_vectorlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TBwaveform>*)
   {
      vector<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TBwaveform>", -2, "c++/v1/vector", 478,
                  typeid(vector<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETBwaveformgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TBwaveform>) );
      instance.SetNew(&new_vectorlETBwaveformgR);
      instance.SetNewArray(&newArray_vectorlETBwaveformgR);
      instance.SetDelete(&delete_vectorlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_vectorlETBwaveformgR);
      instance.SetDestructor(&destruct_vectorlETBwaveformgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TBwaveform> >()));

      ::ROOT::AddClassAlternate("vector<TBwaveform>","std::__1::vector<TBwaveform, std::__1::allocator<TBwaveform> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TBwaveform>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TBwaveform>*)nullptr)->GetClass();
      vectorlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETBwaveformgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBwaveform> : new vector<TBwaveform>;
   }
   static void *newArray_vectorlETBwaveformgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBwaveform>[nElements] : new vector<TBwaveform>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETBwaveformgR(void *p) {
      delete ((vector<TBwaveform>*)p);
   }
   static void deleteArray_vectorlETBwaveformgR(void *p) {
      delete [] ((vector<TBwaveform>*)p);
   }
   static void destruct_vectorlETBwaveformgR(void *p) {
      typedef vector<TBwaveform> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TBwaveform>

namespace ROOT {
   static TClass *vectorlETBmidlETBwaveformgRsPgR_Dictionary();
   static void vectorlETBmidlETBwaveformgRsPgR_TClassManip(TClass*);
   static void *new_vectorlETBmidlETBwaveformgRsPgR(void *p = nullptr);
   static void *newArray_vectorlETBmidlETBwaveformgRsPgR(Long_t size, void *p);
   static void delete_vectorlETBmidlETBwaveformgRsPgR(void *p);
   static void deleteArray_vectorlETBmidlETBwaveformgRsPgR(void *p);
   static void destruct_vectorlETBmidlETBwaveformgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TBmid<TBwaveform> >*)
   {
      vector<TBmid<TBwaveform> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TBmid<TBwaveform> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TBmid<TBwaveform> >", -2, "c++/v1/vector", 478,
                  typeid(vector<TBmid<TBwaveform> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETBmidlETBwaveformgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TBmid<TBwaveform> >) );
      instance.SetNew(&new_vectorlETBmidlETBwaveformgRsPgR);
      instance.SetNewArray(&newArray_vectorlETBmidlETBwaveformgRsPgR);
      instance.SetDelete(&delete_vectorlETBmidlETBwaveformgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlETBmidlETBwaveformgRsPgR);
      instance.SetDestructor(&destruct_vectorlETBmidlETBwaveformgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TBmid<TBwaveform> > >()));

      ::ROOT::AddClassAlternate("vector<TBmid<TBwaveform> >","std::__1::vector<TBmid<TBwaveform>, std::__1::allocator<TBmid<TBwaveform> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TBmid<TBwaveform> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETBmidlETBwaveformgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TBmid<TBwaveform> >*)nullptr)->GetClass();
      vectorlETBmidlETBwaveformgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETBmidlETBwaveformgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETBmidlETBwaveformgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBmid<TBwaveform> > : new vector<TBmid<TBwaveform> >;
   }
   static void *newArray_vectorlETBmidlETBwaveformgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBmid<TBwaveform> >[nElements] : new vector<TBmid<TBwaveform> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETBmidlETBwaveformgRsPgR(void *p) {
      delete ((vector<TBmid<TBwaveform> >*)p);
   }
   static void deleteArray_vectorlETBmidlETBwaveformgRsPgR(void *p) {
      delete [] ((vector<TBmid<TBwaveform> >*)p);
   }
   static void destruct_vectorlETBmidlETBwaveformgRsPgR(void *p) {
      typedef vector<TBmid<TBwaveform> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TBmid<TBwaveform> >

namespace ROOT {
   static TClass *vectorlETBmidlETBfastmodegRsPgR_Dictionary();
   static void vectorlETBmidlETBfastmodegRsPgR_TClassManip(TClass*);
   static void *new_vectorlETBmidlETBfastmodegRsPgR(void *p = nullptr);
   static void *newArray_vectorlETBmidlETBfastmodegRsPgR(Long_t size, void *p);
   static void delete_vectorlETBmidlETBfastmodegRsPgR(void *p);
   static void deleteArray_vectorlETBmidlETBfastmodegRsPgR(void *p);
   static void destruct_vectorlETBmidlETBfastmodegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TBmid<TBfastmode> >*)
   {
      vector<TBmid<TBfastmode> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TBmid<TBfastmode> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TBmid<TBfastmode> >", -2, "c++/v1/vector", 478,
                  typeid(vector<TBmid<TBfastmode> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETBmidlETBfastmodegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TBmid<TBfastmode> >) );
      instance.SetNew(&new_vectorlETBmidlETBfastmodegRsPgR);
      instance.SetNewArray(&newArray_vectorlETBmidlETBfastmodegRsPgR);
      instance.SetDelete(&delete_vectorlETBmidlETBfastmodegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlETBmidlETBfastmodegRsPgR);
      instance.SetDestructor(&destruct_vectorlETBmidlETBfastmodegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TBmid<TBfastmode> > >()));

      ::ROOT::AddClassAlternate("vector<TBmid<TBfastmode> >","std::__1::vector<TBmid<TBfastmode>, std::__1::allocator<TBmid<TBfastmode> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TBmid<TBfastmode> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETBmidlETBfastmodegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TBmid<TBfastmode> >*)nullptr)->GetClass();
      vectorlETBmidlETBfastmodegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETBmidlETBfastmodegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETBmidlETBfastmodegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBmid<TBfastmode> > : new vector<TBmid<TBfastmode> >;
   }
   static void *newArray_vectorlETBmidlETBfastmodegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBmid<TBfastmode> >[nElements] : new vector<TBmid<TBfastmode> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETBmidlETBfastmodegRsPgR(void *p) {
      delete ((vector<TBmid<TBfastmode> >*)p);
   }
   static void deleteArray_vectorlETBmidlETBfastmodegRsPgR(void *p) {
      delete [] ((vector<TBmid<TBfastmode> >*)p);
   }
   static void destruct_vectorlETBmidlETBfastmodegRsPgR(void *p) {
      typedef vector<TBmid<TBfastmode> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TBmid<TBfastmode> >

namespace ROOT {
   static TClass *vectorlETBfastmodegR_Dictionary();
   static void vectorlETBfastmodegR_TClassManip(TClass*);
   static void *new_vectorlETBfastmodegR(void *p = nullptr);
   static void *newArray_vectorlETBfastmodegR(Long_t size, void *p);
   static void delete_vectorlETBfastmodegR(void *p);
   static void deleteArray_vectorlETBfastmodegR(void *p);
   static void destruct_vectorlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TBfastmode>*)
   {
      vector<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TBfastmode>", -2, "c++/v1/vector", 478,
                  typeid(vector<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETBfastmodegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TBfastmode>) );
      instance.SetNew(&new_vectorlETBfastmodegR);
      instance.SetNewArray(&newArray_vectorlETBfastmodegR);
      instance.SetDelete(&delete_vectorlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_vectorlETBfastmodegR);
      instance.SetDestructor(&destruct_vectorlETBfastmodegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TBfastmode> >()));

      ::ROOT::AddClassAlternate("vector<TBfastmode>","std::__1::vector<TBfastmode, std::__1::allocator<TBfastmode> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TBfastmode>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TBfastmode>*)nullptr)->GetClass();
      vectorlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETBfastmodegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBfastmode> : new vector<TBfastmode>;
   }
   static void *newArray_vectorlETBfastmodegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBfastmode>[nElements] : new vector<TBfastmode>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETBfastmodegR(void *p) {
      delete ((vector<TBfastmode>*)p);
   }
   static void deleteArray_vectorlETBfastmodegR(void *p) {
      delete [] ((vector<TBfastmode>*)p);
   }
   static void destruct_vectorlETBfastmodegR(void *p) {
      typedef vector<TBfastmode> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TBfastmode>

namespace {
  void TriggerDictionaryInitialization_libdrcTB_Impl() {
    static const char* headers[] = {
"/Users/swkim/DRC/dual-readout_TB/include/TBdetector.h",
"/Users/swkim/DRC/dual-readout_TB/include/TBevt.h",
"/Users/swkim/DRC/dual-readout_TB/include/TBmid.h",
"/Users/swkim/DRC/dual-readout_TB/include/TBmonit.h",
"/Users/swkim/DRC/dual-readout_TB/include/TBplot.h",
"/Users/swkim/DRC/dual-readout_TB/include/TBread.h",
"/Users/swkim/DRC/dual-readout_TB/include/TButility.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/homebrew/Cellar/python@3.10/3.10.8/Frameworks/Python.framework/Versions/3.10/include/python3.10",
"/Users/swkim/DRC/dual-readout_TB/include",
"/Users/swkim/root-6.26.08/install/include",
"/Users/swkim/DRC/dual-readout_TB",
"/Users/swkim/root-6.26.08/install/include/",
"/Users/swkim/DRC/dual-readout_TB/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libdrcTB dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBwaveform;
class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBfastmode;
class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBmidbase;
template <class T> class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBmid;

class __attribute__((annotate("$clingAutoload$/Users/swkim/DRC/dual-readout_TB/include/TBdetector.h")))  TBcid;
template <class T> class __attribute__((annotate("$clingAutoload$/Users/swkim/DRC/dual-readout_TB/include/TBevt.h")))  TBevt;

)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdrcTB dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/Users/swkim/DRC/dual-readout_TB/include/TBdetector.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBevt.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBmid.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBmonit.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBplot.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TBread.h"
#include "/Users/swkim/DRC/dual-readout_TB/include/TButility.h"
#include "TBmid.h"

#if defined(__CLING__) || defined(__CINT__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TBcid+;
#pragma link C++ class TBwaveform+;
#pragma link C++ class TBfastmode+;

#pragma link C++ class TBmidbase+;
#pragma link C++ class TBmid<TBwaveform>+;
#pragma link C++ class TBmid<TBfastmode>+;
#pragma link C++ class TBevt<TBwaveform>+;
#pragma link C++ class TBevt<TBfastmode>+;

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TBcid", payloadCode, "@",
"TBevt<TBfastmode>", payloadCode, "@",
"TBevt<TBwaveform>", payloadCode, "@",
"TBfastmode", payloadCode, "@",
"TBmid<TBfastmode>", payloadCode, "@",
"TBmid<TBwaveform>", payloadCode, "@",
"TBmidbase", payloadCode, "@",
"TBwaveform", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdrcTB",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdrcTB_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdrcTB_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdrcTB() {
  TriggerDictionaryInitialization_libdrcTB_Impl();
}
