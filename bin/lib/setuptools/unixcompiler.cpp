class unixcompiler : ccompiler
{
	unixcompiler()
	{

	}

    var compiler(var option)
    {
        var cpp = "g++";
        var cpp_flags ;
        var cpp_macros ;
        var ldflags_shared = ["-fPIC","-shared"];
        if(!debug){
            cpp_flags = ["-fPIC","-Wfatal-errors","-std=c++17","-Wno-format-y2k","-fpermissive","-std=gnu++0x","-Wformat-contains-nul","-Wno-unused-function","-O3","-Wall"];
            cpp_macros = ["-DLINUX","-D_FILE_OFFSET_BITS=64","-DNDEBUG"];
        }
        else{
            cpp_flags = ["-Wfatal-errors","-std=c++17","-Wno-format-y2k","-fpermissive","-std=gnu++0x","-Wformat-contains-nul","-Wno-unused-function","-O0","-Wall","-g","-ggdb"];
            cpp_macros = ["-DLINUX","-D_FILE_OFFSET_BITS=64","-DDEBUG"];
        }
        var arch = "x64";
		if(sys.platform == "linux64" )
			arch = "x64";
		else if(sys.platform == "linux32")
			arch = "x86"

        if(arch == "x64")
            cpp_flags.push_back("-m64");

        if(define_macros != null){
			foreach(var macros:define_macros){
				cpp_macros.push_back('-D{macros}');
			}
		}

        var complier_base_libs = ["-lcpps","-lm","-lpthread","-lrt","-ldl"];
        if(libraries != null){
			foreach(var lib:libraries){
				complier_base_libs.push_back('"-l{lib}"');
			}
		}
        
        string.replace(work_path,"\\","/");
        //baseinclude
		var base_include_cpps = '{work_path}../../../include';
		base_include_cpps = io.normpath(base_include_cpps);
		if(string.endswith(base_include_cpps,"\\"))
			string.pop_back(base_include_cpps,1);

        string.replace(base_include_cpps,"\\","/");


		//base deps include
		var base_deps_include_cpps = '/usr/local/include';
		var complier_base_includes = ['-I{base_include_cpps}','-I{base_deps_include_cpps}'];
		if(include_dirs != null){
			foreach(var inc:include_dirs){
				complier_base_includes.push_back('-I{work_path}{inc}');
			}
		}
        
        //baselibpath
        var base_lib_cpps = '{work_path}../../../lib';
		base_lib_cpps = io.normpath(base_lib_cpps);
		if(string.endswith(base_lib_cpps,"\\"))
			string.pop_back(base_lib_cpps,1);
        string.replace(base_lib_cpps,"\\","/");

        var complier_base_lib_path = ['-L{base_lib_cpps}','-L/lib','-L/usr/local/lib','-L/usr/lib','-L/usr/local/lib64','-L/usr/lib64'];
		if(library_dirs != null){
			foreach(var libpath:library_dirs){
				complier_base_lib_path.push_back('-L{libpath}');
			}
		}

         //生成完整cpp 与 obj地址
        sources = option["sources"];
        var newsources = [];
        var objs = [];
        foreach(var p : sources){
        	var fn = io.getfilename(p);
        	var path = io.normpath("{work_path}{io.getfilepath(p)}");
            string.replace(path,"\\","/");
        	var full_path = "{path}{fn}";
        	objs.push_back("{full_path}.o");
        	newsources.push_back(full_path);
        	
        }
        println_color("Building Path:{work_path}",6);

        var c = newsources.size();
        foreach(var i : xrange(0,c-1)){
        	var src = newsources[i];
        	var obj = objs[i];
        	var opt = string.join(" ",cpp_flags);
        	var macos = string.join(" ",cpp_macros);
        	var inc = string.join(" ",complier_base_includes);
        	print("[{i+1}/{c}]");
        	println_color(" Building CXX object {sources[i]} -> {sources[i]}.o",2);
			var cmd = '{cpp} -c {macos} {opt} {inc} -o {obj} {src}';
        	var s = execmd(cmd);
        	if(len(s) > 0 ) { println_color(s,1);}
        }

        println_color("Linking CXX shared library {output_name}.so",3);
        var opt = string.join(" ",ldflags_shared);
        var libs = string.join(" ",complier_base_libs);
        var lib_paths = string.join(" ",complier_base_lib_path);
        var objslist = string.join(" ",objs);
		var cmd = '{cpp} {opt} {lib_paths} -o {work_path}{output_name}.so {libs} {objslist}';
        var s = execmd(cmd);
		if(len(s) > 0)  println_color(s,1);
		else println_color("compiler is done.",2); 

		return 0;
    }
}