#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define UNIT_NUM	100

typedef struct {
	struct kobj_attribute k_obj;
	int num;
} big_kattr;

static struct kobject *register_kobj;

// rw functions
static ssize_t __used store_value(struct kobject *kp, struct kobj_attribute *attr, const char *buf, size_t count){ 
    big_kattr *a = container_of(attr, big_kattr, k_obj);
    sscanf(buf, "%du", &a->num);
    return count;
}

static ssize_t show_value(struct kobject *kp, struct kobj_attribute *attr, char *buf) {
	big_kattr *a = container_of(attr, big_kattr, k_obj);
	return sprintf(buf, "%d\n", a->num);
}

// put attribute to attribute group
static struct attribute * unit_attrs[UNIT_NUM + 1];
static big_kattr full_unit_attrs[UNIT_NUM];
static struct attribute_group  unit_attr_group;

static int hello_init(void){
    int i;
    memset(full_unit_attrs, 0, sizeof(full_unit_attrs));
    memset(unit_attrs, 0, sizeof(unit_attrs));
    memset(&unit_attr_group, 0, sizeof(unit_attr_group));
    
    for(i=0; i<UNIT_NUM; i++){
        char * str = kmalloc(32, GFP_KERNEL);
        sprintf(str, "unit-%03d",i);
	full_unit_attrs[i].k_obj.attr.name = str;
	full_unit_attrs[i].k_obj.attr.mode = S_IWUSR | S_IRUGO;
	full_unit_attrs[i].k_obj.show  = show_value;
	full_unit_attrs[i].k_obj.store = store_value;
	full_unit_attrs[i].num	= i;
	
	unit_attrs[i] = &(full_unit_attrs[i].k_obj.attr);
    }
    unit_attr_group.attrs = unit_attrs;
    // create sysfs object ( /sys/kernel/many directory )
    register_kobj = kobject_create_and_add("many", kernel_kobj);
    if (!register_kobj)
	return -ENOMEM;

    //create all attributes (files)
    if(sysfs_create_group(register_kobj, &unit_attr_group)){
        kobject_put(register_kobj);
        return -ENOMEM;
    }

    return 0;
}

static void hello_exit(void){
    int i;
    kobject_put(register_kobj);
    for(i=0; i<UNIT_NUM; i++)
	kfree(full_unit_attrs[i].k_obj.attr.name);
}

MODULE_LICENSE("Dual BSD/GPL");
module_init(hello_init);
module_exit(hello_exit);

