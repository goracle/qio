#ifndef QIO_H
#define QIO_H

#include <qioxml.h>
#include <qio_string.h>
#include <lrl.h>
#include <dml.h>
#include <qio_config.h>
#include "lime.h"

#define QIO_MASTER_NODE DML_MASTER_NODE

#define QIO_SERIAL     DML_SERIAL     
#define QIO_PARALLEL   DML_PARALLEL   
#define QIO_LEX_ORDER  DML_LEX_ORDER  
#define QIO_NAT_ORDER  DML_NAT_ORDER  
#define QIO_LIST_ORDER DML_LIST_ORDER
#define QIO_SINGLEFILE DML_SINGLEFILE 
#define QIO_MULTIFILE  DML_MULTIFILE  
#define QIO_FIELD      DML_FIELD
#define QIO_GLOBAL     DML_GLOBAL

#define QIO_CREATE     0
#define QIO_TRUNCATE   1
#define QIO_APPEND     2


/* Return codes */

#define QIO_SUCCESS              (  0)
#define QIO_ERR_BAD_WRITE_BYTES  ( -1)
#define QIO_ERR_OPEN_READ        ( -2)
#define QIO_ERR_BAD_READ_BYTES   ( -3)
#define QIO_ERR_ALLOC            ( -4)
#define QIO_ERR_CLOSE            ( -5)
#define QIO_ERR_INFO_MISSED      ( -6)
#define QIO_ERR_BAD_SITE_BYTES   ( -7)
#define QIO_ERR_PRIVATE_REC_INFO ( -8)
#define QIO_BAD_XML              ( -9)
#define QIO_BAD_ARG              (-10)
#define QIO_CHECKSUM_MISMATCH    (-11)
#define QIO_ERR_FILE_INFO        (-12)
#define QIO_ERR_REC_INFO         (-13)
#define QIO_ERR_CHECKSUM_INFO    (-14)
#define QIO_ERR_SKIP             (-15)
#define QIO_ERR_BAD_TOTAL_BYTES  (-16)
#define QIO_ERR_BAD_GLOBAL_TYPE  (-17)

#ifdef __cplusplus
extern "C"
{
#endif
  
/* For collecting and passing layout information */
typedef struct {
  int (*node_number)(const int coords[]);
  int (*node_index)(const int coords[]);
  void (*get_coords)(int coords[], int node, int index);
  int *latsize;
  int latdim;
  size_t volume;
  size_t sites_on_node;
  int this_node;
  int number_of_nodes;
} QIO_Layout;

typedef struct {
  LRL_FileWriter *lrl_file_out;
  int serpar;
  int volfmt;
  DML_Layout *layout;
  DML_SiteRank *sitelist;
} QIO_Writer;

#define QIO_RECORD_XML_NEXT 0
#define QIO_RECORD_DATA_NEXT 1

typedef struct {
  LRL_FileReader *lrl_file_in;
  int serpar;
  int volfmt;
  DML_Layout *layout;
  int siteorder;
  DML_SiteRank *sitelist;
  int read_state;
  QIO_String *xml_record;
  QIO_RecordInfo record_info;
} QIO_Reader;

/* API */
QIO_Writer *QIO_open_write(QIO_String *xml_file, const char *filename, 
			   int serpar, int volfmt, int mode, 
			   QIO_Layout *layout);
QIO_Reader *QIO_open_read(QIO_String *xml_file, const char *filename, 
			   int serpar, QIO_Layout *layout);

int QIO_close_write(QIO_Writer *out);
int QIO_close_read(QIO_Reader *in);

int QIO_write(QIO_Writer *out, QIO_RecordInfo *record_info,
	      QIO_String *xml_record, 
	      void (*get)(char *buf, size_t index, int count, void *arg),
	      size_t datum_size, int word_size, void *arg);
int QIO_read(QIO_Reader *in, QIO_RecordInfo *record_info,
	     QIO_String *xml_record, 
	     void (*put)(char *buf, size_t index, int count, void *arg),
	     size_t datum_size, int word_size, void *arg);
int QIO_read_record_info(QIO_Reader *in, QIO_RecordInfo *record_info,
			 QIO_String *xml_record);
int QIO_read_record_data(QIO_Reader *in, 
		 void (*put)(char *buf, size_t index, int count, void *arg),
		 size_t datum_size, int word_size, void *arg);
int QIO_next_record(QIO_Reader *in);

/* Internal utilities  */
char *QIO_filename_edit(const char *filename, int volfmt, int this_node);
int QIO_write_string(QIO_Writer *out, int msg_begin, int msg_end,
		     QIO_String *xml,
		     const LIME_type lime_type);
int QIO_read_string(QIO_Reader *in, QIO_String *xml, LIME_type lime_type);
int QIO_read_sitelist(QIO_Reader *in, LIME_type lime_type);
int QIO_write_sitelist(QIO_Writer *out, int msg_begin, int msg_end, 
		       const LIME_type lime_type);
  int QIO_read_field(QIO_Reader *in, int globaldata,
	   void (*put)(char *buf, size_t index, int count, void *arg),
	   int count, size_t datum_size, int word_size, void *arg, 
	   DML_Checksum *checksum,
           LIME_type lime_type);
int QIO_write_field(QIO_Writer *out, int msg_begin, int msg_end,
	    QIO_String *xml_record, int globaldata,
	    void (*get)(char *buf, size_t index, int count, void *arg),
	    int count, size_t datum_size, int word_size, void *arg, 
	    DML_Checksum *checksum,
	    const LIME_type lime_type);
#ifdef __cplusplus
}
#endif

#endif /* QIO_H */
