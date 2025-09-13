package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

func main() {
	if len(os.Args) < 3 {
		log.Fatal("Not enough args")
	}

	work_dir := os.Args[1]
	output_dir := fmt.Sprintf("%s/resources", os.Args[2])
	os.Remove(output_dir)
	os.MkdirAll(output_dir, 0755)

	directories := []string{"icons"}

	var source bytes.Buffer
	for _, dir := range directories {
		io.WriteString(&source, fmt.Sprintf("#include \"%s.h\"\n", dir))
		var header bytes.Buffer
		search_dir := fmt.Sprintf("%s/%s", work_dir, dir)
		cmd := exec.Command("find", search_dir, "-type", "f")
		stdout, err := cmd.Output()
		if err != nil {
			log.Fatal(err)
		}
		io.WriteString(&header, fmt.Sprintf("#ifndef %s_HEADER_H\n", strings.ToUpper(dir)))
		io.WriteString(&header, fmt.Sprintf("#define %s_HEADER_H\n", strings.ToUpper(dir)))

		for file := range strings.SplitSeq(strings.TrimSpace(string(stdout)), "\n") {
			file_data, err := os.ReadFile(file)
			if err != nil {
				log.Fatal(err)
			}
			base_name := filepath.Base(file)
			base_name = strings.TrimSuffix(base_name, filepath.Ext(base_name))
			io.WriteString(&header, fmt.Sprintf("extern char %s[];\n", base_name))
			io.WriteString(&source, fmt.Sprintf("char %s[] = {", base_name))
			encoder := hex.NewEncoder(&source)
			for _, b := range file_data {
				io.WriteString(&source, "'\\x")
				encoder.Write([]byte{b})
				io.WriteString(&source, "',")
			}
			io.WriteString(&source, "'\\x00'};\n")
			io.WriteString(&header, fmt.Sprintf("extern unsigned int %s_len;\n", base_name))
			io.WriteString(&source, fmt.Sprintf("unsigned int %s_len = %d;\n", base_name, len(file_data)))
		}

		io.WriteString(&header, fmt.Sprintf("#endif // %s_HEADER_H\n", strings.ToUpper(dir)))
		out_header := fmt.Sprintf("%s/%s.h", output_dir, dir)
		err = os.WriteFile(out_header, header.Bytes(), 0644)
		if err != nil {
			log.Fatal(err)
		}
		out_source := fmt.Sprintf("%s/resources.c", output_dir)
		err = os.WriteFile(out_source, source.Bytes(), 0644)
		if err != nil {
			log.Fatal(err)
		}
	}
}
