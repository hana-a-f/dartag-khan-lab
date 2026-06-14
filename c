##make a haplotype file for mds

./amplicon/to_lep_map.pl -g hap_genotype -f 0.001 -n cj_gala_non_phased_haplotypes -l marker2pos
./amplicon/to_lep_map.pl -g hap_genotype -f 0.001 -n demir_gala_non_phased_haplotypes -l marker2pos


#done, now rename and order so maternal is number 1 and paternal is number two and keep the individuals with the least missing data, did this in R

./amplicon/to_lep_map.pl -g hap_genotype_filt -f 0.001 -m 2 -p 1 -j 'gala_002' -k 'demir_001' -l marker2pos -n Demir_Gala_haplotype

##mfilter in tassel
##run lep map 3

java -cp /programs/Lep-MAP3/bin/ ParentCall2 data = Demir_Gala_haplotype.lepmap3.pedigree.txt vcfFile = Demir_Gala_imputed_phased_af.vcf > post.call
java -cp /programs/Lep-MAP3/bin/ Filtering2 data = post.call removeNonInformative=1 dataTolerance=0.0001 > filtpost.call
java -cp /programs/Lep-MAP3/bin/ SeparateChromosomes2 data=filtpost.call lodLimit=5 sizeLimit=5 > map.txt

#changed to use the physical and see if that improves chromsome 17 and also increased the number of iterations
java -cp /programs/Lep-MAP3/bin/ OrderMarkers2 data=filtpost.call usePhysical=1 sexAveraged=1 map=map.txt > order.txt
  
#tru this to increase the number of iterations
java -cp /programs/Lep-MAP3/bin/ OrderMarkers2 \
  data=filtpost.call \
  map=map.txt \
  sexAveraged=1 \
  usePhysical=1 \
  numMergeIterations=20 \
  numPolishIterations=10 \
  numThreads=8 \
  > order.txt


### Convert order.txt to genotype.txt ### 
  
awk -vfullData=1 -f map2genotypes.awk order.txt > genotypes.txt
awk '(NR>=7)' filtpost.call|cut -f 1,2 > snpnames.txt
paste snpnames.txt map.txt > snplgs.txt





